/**
 * hermes_joint.c
 **/

#include "hermes_joint.h"
#include "hermes_globals.h"
#include "encoder.h"

HermesJoint joints[NUM_JOINTS];

void HermesJoints_init(void) {
  Encoder_init();
  HermesJoint_init(&joints[0], &motor_control, &motor_status, &motor_params, 2);
  return;
}

void HermesJoints_handle(void) {
  Encoder_sample();
  for(int i=0;i<NUM_JOINTS;++i) {
    HermesJoint_handle(&joints[i]);
  }
  return;
}
