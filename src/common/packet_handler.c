/**
 * packet_handler.c
 **/

#include "packet_handler.h"
#include <stdio.h>

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

PacketStatus PacketHandler_sendPacket(PacketHandler* h, PacketHeader* _p) {
  uint8_t* p=(uint8_t*)_p;
  uint8_t* p_end=p+_p->size;
  uint8_t* tx_buf=h->tx_buffer;
  uint8_t* tx_end=&h->tx_end;
  uint8_t* tx_size=&h->tx_size;
  tx_buf[(*tx_end)++]=0xAA;
  ++(*tx_size);
  tx_buf[(*tx_end)++]=0x55;
  ++(*tx_size);
  while(p<p_end) {
    tx_buf[(*tx_end)++]=*p;
    ++(*tx_size);
    ++p;
  }
}

PacketStatus PacketHandler_readByte(PacketHandler* h, uint8_t c) {
  return (*h->receive_fn)(h, c);
}

uint8_t PacketHandler_writeByte(PacketHandler* h) {
  if(h->tx_size==0)
    return 0;
  uint8_t c=h->tx_buffer[h->tx_start++];
  h->tx_size--;
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
    h->rx_buffer=h->packet_op[c].rx_buf;
    h->rx_size=h->packet_op[c].rx_size;
    h->receive_fn=_rxSize;
    return Success;
  }
}
PacketStatus _rxSize(PacketHandler* h, uint8_t c) {
  if(c!=h->rx_size) {
    h->receive_fn=&_rxAA;
    h->rx_buffer=0;
    h->rx_size=0;
    return WrongSize;
  } else {
    h->bytes_to_read=c; //
    h->receive_fn=_rxPayload;
    return Success;
  }
}

PacketStatus _rxPayload(PacketHandler* h, uint8_t c) {
  h->rx_buffer[h->rx_end++]=c;
  h->bytes_to_read--;
  if(h->bytes_to_read==0) {
    h->receive_fn=_rxCs;
    return (*h->receive_fn)(h, h->rx_checksum);
  }
  return Success;
}

PacketStatus _rxCs(PacketHandler* h, uint8_t c) {
  // TODO
  (*h->packet_op[h->curr_id].on_receive_fn)(h->packet_op[h->curr_id].args);
  h->receive_fn=_rxAA;
  
  return Success;
}
