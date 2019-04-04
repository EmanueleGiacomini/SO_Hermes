/**
 * hermes_comm.c
 **/

#include "hermes_comm.h"

#define PACKET_BUFFER_SIZE 8

MotorControlPacket motor_control_packet_buffer[PACKET_BUFFER_SIZE];
MotorStatusPacket motor_status_packet_buffer[PACKET_BUFFER_SIZE];

PacketStatus HermesComm_writePacket(HermesComm*, PacketHeader*);

PacketStatus HermesComm_readPacket(HermesComm*, PacketHeader*);

void HermesComm_handle(HermesComm*);
