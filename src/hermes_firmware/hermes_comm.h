/**
 * hermes_comm.h
 **/

#pragma once
#include "hermes_packets.h"
#include "packet_handler.h"
#include "packet_status.h"

typedef struct {
  PacketHandler phandler;
  
}HermesComm;

PacketStatus HermesComm_writePacket(HermesComm*, PacketHeader*);

PacketStatus HermesComm_readPacket(HermesComm*, PacketHeader*);

void HermesComm_handle(HermesComm*);


