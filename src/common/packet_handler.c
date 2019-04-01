/**
 * packet_handler.c
 **/

#include "packet_handler.h"

PacketStatus _rxAA(PacketHandler* h, uint8_t c);
PacketStatus _rx55(PacketHandler* h, uint8_t c);
PacketStatus _rxId(PacketHandler* h, uint8_t c);
PacketStatus _rxSize(PacketHandler* h, uint8_t c);
PacketStatus _rxPayload(PacketHandler* h, uint8_t c);
PacketStatus _rxCs(PacketHandler* h, uint8_t c); // checksum




void _flushTxBuffer(PacketHandler* h) {
  uint8_t* buf=h->tx_buffer;
  uint8_t* bsize=&h->tx_size;
  uint8_t* txs=&h->tx_start;
  while(bsize) {
    Uart_write(h->uart, buf[*(txs)++]);
    --*(bsize);
  }
}

void PacketHandler_init(PacketHandler* h) {
  h->tx_start=0;
  h->tx_end=0;
  h->tx_size=0;
  h->uart=Uart_init(115200);
}

void PacketHandler_addOperation(PacketHandler* h, PacketOperation* o) {
  h->packet_op[o->id]=*o;
}

PacketStatus PacketHandler_sendPacket(PacketHandler* h, PacketHeader* p) {
  uint8_t psize=p->size;
  uint16_t start_idx=(uint16_t)p;
  uint16_t end_idx=(uint16_t)p+sizeof(PacketHandler)+psize;
  uint8_t tx_size=h->
  uint8_t* tx_buffer=h->tx_buffer;
  while(start_idx<end_idx) {
    tx_buffer[start_idx++]=*(uint8_t*)p;
    ++p;
    ++start_idx;
  }
  return Success;
}

PacketStatus PacketHandler_readByte(PacketHandler* h, uint8_t c) {
  return (*h->receive_fn)(h, c);
}

uint8_t PacketHandler_writeByte(PacketHandler* h) {
  if(h->tx_size==0)
    return 0;
  
}


PacketStatus _rxAA(PacketHandler* h, uint8_t c) {
  if(c==0xAA)
    h->receive_fn=_rx55;
  return Success;
}

PacketStatus _rx55(PacketHandler* h, uint8_t c) {
  if(c==0x55)
    h->receive_fn=_rxId;
  else
    h->receive_fn=_rxAA;
  return Success;
}

PacketStatus _rxId(PacketHandler* h, uint8_t c) {
  if(c>=MAX_PACKET_TYPE) {
    h->receive_fn=&_rxAA;
    return UnknownType;
  } else {
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
    h->bytes_to_read=c+4; // + 4 for seq, dest_addr, src_addr, checksum
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
  h->receive_fn=_rx55;
  return Success;
}
