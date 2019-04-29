/**
 * pwm.h
 **/

#pragma once
#include <stdint.h>

void PWM_init(void);

void PWM_enablePin(uint8_t);

void PWM_setOutput(uint8_t, uint8_t);
