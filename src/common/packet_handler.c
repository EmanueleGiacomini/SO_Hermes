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
  h->packet_op[o->id]=*o;
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
}

PacketStatus PacketHandler_readByte(PacketHandler* h, uint8_t c) {
  return (*h->receive_fn)(h, c);
}

uint8_t PacketHandler_writeByte(PacketHandler* h) {
  if(h->tx_size==0)
    return 0;
  uint8_t c=buffer_read(h->tx_buffer, &h->tx_start, PACKET_SIZE_MAX);
  --h->tx_size;
  return c;
}


PacketStatus _rxAA(PacketHandler* h, uint8_t c) {
  if(c==0xAA) {
    h->receive_fn=_rx55;
  }
  return Success;
}

PacketStatus _rx55(PacketHandler* h, uint8_t c) {
  if(c==0x55) {
    h->receive_fn=_rxId;
  }
  else {
    h->receive_fn=_rxAA;
  }
  return Success;
}

PacketStatus _rxId(PacketHandler* h, uint8_t c) {
  if(c>=MAX_PACKET_TYPE) {
    h->receive_fn=&_rxAA;
    return UnknownType;
  } else {
    h->curr_id=c;
    h->rx_buffer=(h->packet_op[c].rx_buf);
    h->receive_fn=_rxSize;
    return Success;
  }
}
PacketStatus _rxSize(PacketHandler* h, uint8_t c) {
  if(c!=h->packet_op[h->curr_id].exp_size) {
    h->receive_fn=&_rxAA;
    h->rx_buffer=0;
    h->curr_packet=0;
    h->rx_end=0;
    h->rx_size=0;
    return WrongSize;
  } else {
    h->rx_end=h->packet_op[h->curr_id].rx_end;
    h->rx_size=h->packet_op[h->curr_id].rx_size;
    h->curr_packet=h->rx_buffer+h->rx_end;
    buffer_insert(h->rx_buffer, &h->rx_end, h->rx_size, h->curr_id);
    h->rx_checksum=h->curr_id;
    buffer_insert(h->rx_buffer, &h->rx_end, h->rx_size, c);
    h->rx_checksum^=c;
    h->bytes_to_read=c-2; // because we already read ID and SIZE
    h->receive_fn=_rxPayload;
    return Success;
  }
}

PacketStatus _rxPayload(PacketHandler* h, uint8_t c) {
  h->rx_checksum^=c;
  buffer_insert(h->rx_buffer, &h->rx_end, h->rx_size, c);
  h->bytes_to_read--;
  if(h->bytes_to_read==0) {
    h->receive_fn=_rxCs;
    return (*h->receive_fn)(h, h->rx_checksum);
  }
  return Success;
}

PacketStatus _rxCs(PacketHandler* h, uint8_t c) {
  // TODO
  
  uint8_t recv_cs=h->rx_checksum;
  recv_cs^=((PacketHeader*)(h->curr_packet))->checksum;
  ((PacketHeader*)(h->curr_packet))->checksum=0;
  uint8_t computed_cs=computeCS((PacketHeader*)h->curr_packet);
  ((PacketHeader*)(h->curr_packet))->checksum=recv_cs;
  if(recv_cs!=computed_cs) {
    h->receive_fn=_rxAA;
    return ChecksumError;
  }
  (*h->packet_op[h->curr_id].on_receive_fn)(h->packet_op[h->curr_id].args);
  h->receive_fn=_rxAA;
  
  return Success;
}
