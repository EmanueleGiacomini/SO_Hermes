/**
 * pwm.c
 **/

#include "pwm.h"
#include "pins.h"
#include <avr/io.h>
#include <avr/interrupt.h>

void PWM_init(void) {
  // Set TIMER0 and TIMER2 (8 bit timers)
  TCCR0A=(1<<WGM00);
  TCCR0B=(1<<CS00)|(1<<CS01);
  TCCR2A=TCCR0A;
  TCCR2B=TCCR0B;
}

void PWM_enablePin(uint8_t pin) {
  if(_pins[pin].tccr==0)
    return;
  *(_pins[pin].tccr)|=(_pins[pin].mask);
  *(_pins[pin].ocr)=0;
  return;
}

void PWM_setOutput(uint8_t pin, uint8_t pwm) {
  if(_pins[pin].ocr==0)
    return;
  if(pwm>255)
    pwm=255;
  *(_pins[pin].ocr)=pwm;
}
