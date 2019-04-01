/**
 * packet_test.c
 **/

#include "hermes_packets.h"
#include "uart.h"
#include "packet_handler.h"
#include <stdio.h>
#include <stdlib.h>


MotorControlPacket motor_control_packet;
PacketOperation motor_control_packet_ops={
  .id=ID_MOTOR_CONTROL_PACKET,
  .rx_buf=(uint8_t*)&motor_control_packet,
  .rx_size=sizeof(MotorControlPacket),
  .rx_start=0,
  .rx_end=0,
  .on_receive_fn=&receiveFn,
  .args=0,
};

PacketHandler packet_handler;

int main(int argc, char** argv) {
  PacketHandler_init()
  
  return 0;
}
