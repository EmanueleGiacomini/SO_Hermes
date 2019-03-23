/**
 * packet_handler.c
 **/

#include "packet_handler.h"

PacketStatus _rxAA(PacketHandler* h, uint8_t c);
PacketStatus _rx55(PacketHandler* h, uint8_t c);
PacketStatus _rxSeq(PacketHandler* h, uint8_t c);
PacketStatus _rxSeq(PacketHandler* h, uint8_t c);


void _flushTxBuffer(PacketHandler* h) {
  uint8_t* buf=h->tx_buffer;
  uint8_t* bsize=&h->tx_size;
  uint8_t* txs=h->tx_start;
  while(bsize) {
    uart_write(h->uart, buf[*(txs)++]);
    --*(bsize);
  }
}

void PacketHandler_init(PacketHandler* h) {
  h->tx_start=0;
  h->tx_end=0;
  h->tx_size=0;
  h->uart=Uart_init(115200);
}

PacketStatus PacketHandler_sendPacket(PacketHandler* h, PacketHeader* p) {
  uint8_t psize=p->size;
  uint16_t start_idx=p;
  uint8_t 
  uint16_t end_idx=p+sizeof(PacketHandler)+psize;
  while(start_idx<end_idx) {
    tx_buffer[start_idx++]=*p;
    ++p;
    ++start_idx;
  }
  return Success;
}

PacketStatus PacketHandler_readByte(PacketHandler* h, uint8_t c) {
  return Success;
}
