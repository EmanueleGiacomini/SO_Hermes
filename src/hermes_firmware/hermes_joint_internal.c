/**
 * hermes_joint_internal.c
 **/

#include "hermes_joint_internal.h"

void HermesJoint_init(HermesJoint* j, MotorControlPacket* _control,
                      MotorStatusPacket* _status,
                      MotorParamsPacket* _param) {
  j->control=_control;
  j->status=_status;
  j->params=_params;

  // initialize PWM
  PWM_init();
  PWM_enablePin(j->params.pwm_pin);
  PWM_setOutput(j->params.pwm_pin, 0);
  // Initialize DigIO
  digio_configurePin(j->params.dir_pin, Output);
  digio_setPin(j->params.dir_pin, 0);
  return;
}

uint16_t clamp(uint16_t v, uint16_t max) {
  if(v>max)
    return max;
  if(v<-max)
    return -max;
  return v;
}

void HermesJoint_handle(HermesJoint* j) {
  // Encoder section (j->h.dest_addr is the joint index)
  Encoder_sample();
  uint8_t enc_idx=j->h.dest_addr;
  // for now
  enc_idx=0;
  int16_t prev_ticks=j->status.encoder_ticks;
  j->status.encoder_ticks=Encoder_getValue(enc_idx);
  j->measured_speed=j->status.encoder_ticks-prev_ticks;
  
  
  if(j->control.mode=Disabled)
    return;
  if(j->control.mode==Pid) {
    // Pid Mode
    return;
  }
  if(j->control.mode==Direct) {
    // Direct Mode
    int16_t speed=j->control.speed;
    uint8_t dir=0;
    if(speed<0) {
      dir=1;
      speed=-speed;
    }
    speed=clamp(speed, 255);
    j->dir=dir;
    j->output=speed;
    digio_setPin(j->params.dir_pin, j->dir);
    PWM_setOutput(j->parms.pwm_pin, j->output);
    return;
  }                        
  return;
}

