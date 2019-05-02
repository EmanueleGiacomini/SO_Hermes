/**
 * hermes_joint_internal.c
 **/

#include "hermes_joint_internal.h"
#include "digio.h"

void HermesJoint_init(HermesJoint* j, MotorControlPacket* _control,
                      MotorStatusPacket* _status,
                      MotorParamsPacket* _params,
                      uint8_t _eidx) {
  j->control=_control;
  j->status=_status;
  j->params=_params;
  j->enc_idx=_eidx;

  // initialize PWM
  PWM_init();
  digio_configurePin(j->params->pwm_pin, Output);
  digio_setPin(j->params->pwm_pin, 0);
  PWM_enablePin(j->params->pwm_pin);
  PWM_setOutput(j->params->pwm_pin, 0);
  // Initialize DigIO
  digio_configurePin(j->params->dir_a_pin, Output);
  digio_setPin(j->params->dir_a_pin, 0);
  digio_configurePin(j->params->dir_b_pin, Output);
  digio_setPin(j->params->dir_b_pin, 0);
  return;
}

int16_t clamp(int16_t v, int16_t max) {
  if(v>max)
    return max;
  if(v<-max)
    return -max;
  return v;
}

void HermesJoint_handle(HermesJoint* j) {
  // Encoder Section
  uint8_t enc_idx=j->enc_idx;
  int16_t prev_ticks=j->status->encoder_ticks;
  j->status->encoder_ticks=Encoder_getValue(enc_idx);
  j->status->measured_speed=j->status->encoder_ticks-prev_ticks;
  
  if(j->control->mode==Disabled)
    return;
  if(j->control->mode==Pid) {
    // Pid Mode
    return;
  }
  if(j->control->mode==Direct) {
    // Direct Mode
    int16_t speed=j->control->speed;
    uint8_t dir=0;
    speed=clamp(speed, 255);
    j->status->desired_speed=speed;
    if(speed<0) {
      dir=1;
      speed=-speed;
    }    
    j->dir=dir;
    j->output=speed;
    digio_setPin(j->params->dir_a_pin, j->dir);
    digio_setPin(j->params->dir_b_pin, !j->dir);
    PWM_setOutput(j->params->pwm_pin, j->output);
    return;
  }                        
  return;
}

