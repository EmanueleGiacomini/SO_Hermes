/**
 * pwm_test.c
 **/

#include "digio.h"
#include "pwm.h"
#include "delay.h"
#include <stdint.h>

int main(int argc, char* argv[]) {
  PWM_init();
  digio_configurePin(4, Output);
  PWM_enablePin(4);
  
  while(1) {
    int i=0;
    for(i=0;i<255;++i) {
      PWM_setOutput(4, i);
      delay(5);
    }
    
    for(i=255;i>0;--i) {
      PWM_setOutput(4, i);
      delay(5);
    }
    
  }
  
  return 0;
}
