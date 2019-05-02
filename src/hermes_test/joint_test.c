/**
 * joint_test.c
 **/

#include <stdint.h>
#include "digio.h"
#include "delay.h"

#include "hermes_comm.h"
#include "hermes_globals.h"
#include "hermes_joint.h"
#include "uart.h"


static uint8_t led_state=0;

void testFn(void) {
  digio_setPin(13, led_state);
  led_state=!led_state;
}

int main(int argc, char* argv[]) {
  Encoder_init();
  HermesJoints_init();
  Timer_init();

  struct Uart* u1=Uart_init(115200);
  HermesComm_init(O_UART | O_NRF24L01);

  //digio_configurePin(13, Output);

  //struct Timer* timer_test=Timer_create(100, &testFn, 0);
  struct Timer* timer_comm=Timer_create(100, &HermesComm_handle, 0);
  struct Timer* timer_joint=Timer_create(100, &HermesJoints_handle, 0);
  
  //Timer_start(timer_test);
  Timer_start(timer_comm);
  //Timer_start(timer_joint);

  motor_control.mode=Direct;
  
  while(1) {
    for(int i=0;i<255;++i) {
      motor_control.speed=i;
      HermesJoints_handle();
      Uart_write(u1, motor_status.desired_speed);
      //Uart_write(u1, motor_control.speed);
      delay(100);
    }
    // stuff
  }
}
