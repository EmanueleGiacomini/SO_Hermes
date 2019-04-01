/**
 * packet_operations.h
 **/
#pragma once
#include <stdint.h>
#include "hermes_packets.h"

typedef struct PacketOperation{
  uint8_t id;
  uint8_t* rx_buf;
  uint8_t rx_size;
  uint8_t rx_start;
  uint8_t rx_end;
  void* on_receive_fn;
  void* args;
}PacketOperation;
