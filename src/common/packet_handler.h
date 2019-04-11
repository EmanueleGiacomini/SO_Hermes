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

typedef struct {
  PacketOperation packet_ops[MAX_PACKET_TYPE];
  // transmission
  uint8_t tx_buffer[PACKET_SIZE_MAX];
  uint8_t* tx_start;
  uint8_t* tx_end;
  uint16_t tx_size;

  // acquisition
  PacketOperation* current_op;
  PacketHeader* current_packet;
  uint8_t rx_buffer[PACKET_SIZE_MAX];
  uint8_t* rx_start;
  uint8_t* rx_end;
  uint8_t rx_checksum;
  
  PacketHandlerFn receive_fn;
}PacketHandler;

void PacketHandler_init(PacketHandler*);

void PacketHandler_addOperation(PacketHandler*, PacketOperation*);

PacketStatus PacketHandler_sendPacket(PacketHandler*, PacketHeader*);

PacketStatus PacketHandler_readByte(PacketHandler*, uint8_t);

uint8_t PacketHandler_txSize(PacketHandler*);

uint8_t PacketHandler_writeByte(PacketHandler*);
