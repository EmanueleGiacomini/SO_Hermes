/**
 * packet_handler.c
 **/

#include "packet_handler.h"
#include <stdio.h>
#include "buffer.h"

PacketStatus _rxAA(PacketHandler* h, uint8_t c);
PacketStatus _rx55(PacketHandler* h, uint8_t c);
PacketStatus _rxId(PacketHandler* h, uint8_t c);
PacketStatus _rxSize(PacketHandler* h, uint8_t c);
PacketStatus _rxPayload(PacketHandler* h, uint8_t c);
PacketStatus _rxCs(PacketHandler* h, uint8_t c); // checksum

void PacketHandler_init(PacketHandler* h) {
  h->tx_start=0;
  h->tx_end=0;
  h->tx_size=0;
  h->receive_fn=_rxAA;
}

void PacketHandler_addOperation(PacketHandler* h, PacketOperation* o) {
  h->packet_ops[o->id]=*o;
}

static uint8_t computeCS(PacketHeader* _p) {
  uint8_t checksum=0;
  uint8_t* p=(uint8_t*)_p;
  for(int i=0;i<_p->size;++i) {
    checksum^=p[i];
  }
  return checksum;
}

PacketStatus PacketHandler_sendPacket(PacketHandler* h, PacketHeader* _p) {
  uint8_t cs=computeCS(_p);
  _p->checksum=cs;
  uint8_t*p=(uint8_t*)_p;
  uint8_t*p_end=p+_p->size;

  h->tx_end=h->tx_buffer; // reset buffer start/end
  h->tx_start=h->tx_buffer;

  uint8_t tx_size=2;
  uint8_t* tx_end=h->tx_end;
  *tx_end=0xAA;//insert 0xAA and 0x55 to initialize protocol
  tx_end++;
  *tx_end=0x55;
  tx_end++;
  while(p!=p_end) {
    *tx_end=*p;
    ++tx_size;
    ++tx_end;
    ++p;    
  }
  h->tx_size=tx_size;
  return Success;
  /**
  uint8_t checksum=computeCS(_p);
  _p->checksum=checksum;
  uint8_t* p=(uint8_t*)_p;
  uint8_t* p_end=p+_p->size;
  uint8_t* tx_buf=(uint8_t*)h->tx_buffer;
  uint16_t* tx_end=&h->tx_end;
  uint16_t* tx_size=&h->tx_size;
  buffer_insert(tx_buf, tx_end, PACKET_SIZE_MAX, 0xAA);
  ++(*tx_size);
  buffer_insert(tx_buf, tx_end, PACKET_SIZE_MAX, 0x55);
  ++(*tx_size);
  while(p<p_end) {
    buffer_insert(tx_buf, tx_end, PACKET_SIZE_MAX, *p);
    ++(*tx_size);
    ++p;
  }
  return Success;
  **/
}

PacketStatus PacketHandler_readByte(PacketHandler* h, uint8_t c) {
  return (*h->receive_fn)(h, c);
}

uint8_t PacketHandler_txSize(PacketHandler* h) {
  return h->tx_size;
}

uint8_t PacketHandler_writeByte(PacketHandler* h) {
  if(h->tx_size==0)
    return 0;
  uint8_t *tx_start=h->tx_start;
  uint8_t c=*tx_start;
  ++h->tx_start;
  --h->tx_size;
  return c;
}


PacketStatus _rxAA(PacketHandler* h, uint8_t c) {
  if(c==0xAA) {
    h->receive_fn=_rx55;
    return Success;
  }
  return UnknownType;
}

PacketStatus _rx55(PacketHandler* h, uint8_t c) {
  if(c==0x55) {
    h->receive_fn=_rxId;
    return Success;
  }
  h->receive_fn=_rxAA;
  return UnknownType;
}

PacketStatus _rxId(PacketHandler* h, uint8_t c) {
  if(c>=MAX_PACKET_TYPE) {
    h->receive_fn=_rxAA;
    return UnknownType;
  }
  h->current_op=&h->packet_ops[c];
  h->receive_fn=_rxSize;
  return Success;
}
PacketStatus _rxSize(PacketHandler* h, uint8_t c) {
  if(h->current_op->size!=c) {
    h->receive_fn=_rxAA;
    h->current_op=0;
    return WrongSize;
  }
  // inserting id and size
  h->rx_end=h->rx_buffer+c; // buffer_end = buffer_start + packet_size
  h->rx_start=h->rx_buffer;
  h->rx_buffer[0]=h->current_op->id;
  h->rx_buffer[1]=c;
  h->rx_start+=2;
  // computing checksum
  h->rx_checksum=h->rx_buffer[0];
  h->rx_checksum^=c;
  h->receive_fn=_rxPayload;
  return Success;
}

PacketStatus _rxPayload(PacketHandler* h, uint8_t c) {
  h->rx_checksum^=c; // computing checksum
  (*h->rx_start)=c; // inserting byte
  h->rx_start++;
  if(h->rx_end==h->rx_start) {
    h->rx_start=0;
    h->rx_end=0;
    h->receive_fn=_rxCs; // launching ChecksumFn
    return (*h->receive_fn)(h, h->rx_checksum);
  }
  return Success;
}

PacketStatus _rxCs(PacketHandler* h, uint8_t c) {
  h->current_packet=(PacketHeader*)h->rx_buffer;
  uint8_t recv_cs=h->rx_checksum; // received checksum
  recv_cs^=h->current_packet->checksum; // transmitter computed checksum
  h->current_packet->checksum=0;
  uint8_t computed_cs=computeCS(h->current_packet);
  h->current_packet->checksum=recv_cs;
  
  if(recv_cs!=computed_cs) {
    h->receive_fn=_rxAA;
    return ChecksumError;
  }
  receiveFn_t recvFn=h->current_op->on_receive_fn;
  (*recvFn)(h->current_packet, h->current_op->args);
  h->receive_fn=_rxAA;
  return Success;
}
