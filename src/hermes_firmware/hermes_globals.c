/**
 * hermes_globals.c
 **/

#include "hermes_globals.h"

MotorControlPacket motor_control={
  {
    .id=ID_MOTOR_CONTROL_PACKET,
    .size=sizeof(motor_control),
    .seq=0,
    .dest_addr=0,
    .src_addr=0,
    .checksum=0,
  },
  .mode=0,
  .speed=0
};

MotorStatusPacket motor_status={
  {
    .id=ID_MOTOR_STATUS_PACKET,
    .size=sizeof(motor_status),
    .seq=0,
    .dest_addr=0,
    .src_addr=0,
    .checksum=0,
  },
  .encoder_ticks=0,
  .desired_speed=0,
  .measured_speed=0
};

SystemStatusPacket system_status={
  {
    .id=ID_SYSTEM_STATUS_PACKET,
    .size=sizeof(system_status),
    .seq=0,
    .dest_addr=0,
    .src_addr=0,
    .checksum=0,
  },
  .rx_packets=0,
  .rx_errors=0,
  .tx_packets=0,
  .idle_cycles=0
};
