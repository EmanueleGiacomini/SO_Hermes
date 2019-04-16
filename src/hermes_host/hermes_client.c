/**
 * hermes_client.c
 * This library aims to offer a solid connection between
 **/

#include "hermes_client.h"
#include "packet_handler.h"
#include "packet_operations.h"
#include "serial.h"
#include <stdio.h>

#define PACKET_BUFFER_SIZE 8

void HermesClient_init(HermesClient*, char* dev, int speed);
PacketStatus HermesClient_sendPacket(HermesClient*, PacketHeader*);
PacketStatus HermesClient_readPacket(HermesClient*, PacketHeader*);

int HermesClient_init(char* dev, int speed) { 
  int ret;
  serial_fd=setupSerial(dev, speed);
  if(serial_fd<0) {
    return -1;
  }
  PacketHandler_init(&packet_handler);
  return serial_fd;
}

PacketStatus HermesClient_sendPacket(PacketHeader* h) {
  PacketHandler_sendPacket(&packet_handler, h);
  return Success;
}

PacketStatus HermesClient_readPacket(PacketHeader* h) {
  return Success;
}
