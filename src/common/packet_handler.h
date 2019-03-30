/**
 * packet_handler.h
 **/
#pragma once
#include "stdint.h"
#include "hermes_packets.h"
#include "packet_status.h"
#include "packet_operations.h"
#include "uart.h"

struct PacketHandler;

typedef PacketStatus (*PacketHandlerFn)(struct PacketHandler*, uint8_t);

typedef struct PacketHandler{
  PacketOperations packet_op[MAX_PACKET_TYPE];
  uint8_t tx_buffer[PACKET_SIZE_MAX];
  uint8_t tx_start;
  uint8_t tx_end;
  uint8_t tx_size;
  struct Uart *uart;

  uint8_t* rx_buffer;
  uint8_t rx_size;
  uint8_t rx_start;
  uint8_t rx_end;

  PacketHandlerFn* receive_fn;
}PacketHandler;

void PacketHandler_init(PacketHandler*);

PacketStatus PacketHandler_sendPacket(PacketHandler*, PacketHeader*);

PacketStatus PacketHandler_readByte(PacketHandler*, uint8_t);
