/**
 * packet_operations.h
 **/
#pragma once
#include <stdint.h>
#include "hermes_packets.h"

typedef struct PacketOperations{
  uint8_t* receive_buf;
  uint8_t* receive_buf_size;
}PacketOperations;
