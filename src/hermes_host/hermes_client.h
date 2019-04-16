/**
 * hermes_client.h
 * This library aims to offer a solid connection between
 **/

#pragma once
#include "hermes_packets.h"
#include "packet_status.h"


typedef struct {
  MotorControlPacket motor_control;
  MotorStatusPacket motor_status;
  SystemStatusPacket system_status;

  PacketHandler ph;//packet handler
  int serialfd=0;//uart fd
}HermesClient;


void HermesClient_init(HermesClient*, char* dev, int speed);
PacketStatus HermesClient_sendPacket(HermesClient*, PacketHeader*);
PacketStatus HermesClient_readPacket(HermesClient*, PacketHeader*);




