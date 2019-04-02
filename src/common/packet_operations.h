/**
 * packet_operations.h
 **/
#pragma once
#include <stdint.h>
#include "hermes_packets.h"

typedef void (*receiveFn_t)(void*);

typedef struct PacketOperation{
  uint8_t id;
  uint8_t* rx_buf;
  uint8_t rx_size;
  uint8_t rx_start;
  uint8_t rx_end;
  receiveFn_t on_receive_fn;
  void* args;
}PacketOperation;
