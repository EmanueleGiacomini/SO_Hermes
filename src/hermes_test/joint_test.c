/**
 * joint_test.c
 **/

#include <stdint.h>
#include "digio.h"
#include "delay.h"

#include "hermes_comm.h"
#include "hermes_globals.h"
#include "hermes_joint.h"


static uint8_t led_state=0;

void testFn(void) {
  digio_setPin(13, led_state);
  led_state=!led_state;
}

int main(int argc, char* argv[]) {
  //HermesJoints_init();
  Timer_init();
  HermesComm_init(O_UART | O_NRF24L01);

  digio_configurePin(13, Output);

  struct Timer* timer_test=Timer_create(100, &testFn, 0);
  struct Timer* timer_comm=Timer_create(100, &HermesComm_handle, 0);
  
  Timer_start(timer_test);
  Timer_start(timer_comm);
  
  while(1) {
    // stuff
  }
}
