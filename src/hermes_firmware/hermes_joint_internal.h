/**
 * hermes_joint_internal.h
 **/

#pragma once
#include <stdint.h>
#include "hermes_packets.h"

typedef struct {
  MotorControlPacket* control;
  MotorStatusPacket* status;
  MotorParamsPacket* params;
  uint8_t dir;
  uint8_t output;
}HermesJoint;


void HermesJoint_init(HermesJoint*, MotorControlPacket*,
                      MotorStatusPacket*,
                      MotorParamsPacket*);

void HermesJoint_handle(HermesJoint*);

