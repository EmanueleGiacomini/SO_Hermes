/**
 * packet_handler.h
 **/
#pragma once
#include "stdint.h"
#include "hermes_packets.h"
#include "packet_status.h"
#include "uart.h"

typedef struct {  
  uint8_t tx_buffer[PACKET_SIZE_MAX];
  uint8_t tx_start;
  uint8_t tx_end;
  uint8_t tx_size;
  struct Uart *uart;
}PacketHandler;

void PacketHandler_init(PacketHandler*);

PacketStatus PacketHandler_sendPacket(PacketHandler*, PacketHeader*);

PacketStatus PacketHandler_readByte(PacketHandler*, uint8_t);
