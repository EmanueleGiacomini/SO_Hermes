/**
 * packet_handler.h
 **/
#pragma once
#include "stdint.h"
#include "hermes_packets.h"
#include "packet_status.h"
#include "packet_operations.h"

struct PacketHandler;

typedef PacketStatus (*PacketHandlerFn)(struct PacketHandler*, uint8_t);

typedef struct PacketHandler{
  PacketOperation packet_op[MAX_PACKET_TYPE];
  uint8_t tx_buffer[PACKET_SIZE_MAX];
  uint8_t tx_start;
  uint8_t tx_end;
  uint8_t tx_size;

  uint8_t curr_id;
  uint8_t* rx_buffer;
  uint8_t rx_size;
  uint8_t rx_start;
  uint8_t rx_end;
  uint8_t bytes_to_read;
  uint8_t rx_checksum;

  PacketHandlerFn receive_fn;
}PacketHandler;

void PacketHandler_init(PacketHandler*);

void PacketHandler_addOperation(PacketHandler*, PacketOperation*);

PacketStatus PacketHandler_sendPacket(PacketHandler*, PacketHeader*);

PacketStatus PacketHandler_readByte(PacketHandler*, uint8_t);

uint8_t PacketHandler_writeByte(PacketHandler*);
