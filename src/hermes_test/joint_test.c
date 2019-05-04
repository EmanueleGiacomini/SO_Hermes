/**
 * joint_test.c
 **/

#include <stdint.h>
#include "digio.h"
#include "delay.h"

#include "hermes_comm.h"
#include "hermes_globals.h"
#include "hermes_joint.h"
#include "encoder.h"
#include "timer.h"
#include "uart.h"

volatile uint16_t icycles=0;// idle cycles

volatile uint8_t comm_flag=0;

void timerCommFn(void) {
  comm_flag=1;
}

struct Uart* u1;

void commFn(void) {
  system_status.idle_cycles=icycles;
  icycles=0;
  HermesComm_handle();
  HermesComm_readPacket((PacketHeader*)&motor_control);
  HermesJoints_handle();
  HermesComm_sendPacket((PacketHeader*)&motor_status, O_UART);
  
  comm_flag=0;
}



int main(int argc, char* argv[]) {
  Encoder_init();
  PWM_init();
  HermesJoints_init();
  Timer_init();
  HermesComm_init(O_UART);

  u1=Uart_init(115200);

  struct Timer* timer_comm=Timer_create(10, &timerCommFn, 0);
  
  Timer_start(timer_comm);
  
  while(1) {
    ++icycles;
    if(comm_flag)
      commFn();
  }
}
