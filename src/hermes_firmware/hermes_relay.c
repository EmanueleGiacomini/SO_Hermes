/**
 * hermes_client.c
 *
 * relay between host and client, trasmitting to client
 **/

#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "uart.h"
#include "delay.h"
#include "digio.h"
#include "spi.h"
#include "timer.h"
#include "hermes_nRF24L01.h"

#include "hermes_packets.h"
#include "packet_handler.h"
#include "hermes_globals.h"
#include "hermes_comm.h"


volatile uint8_t comm_flag=0;
volatile uint8_t joint_flag=0;
volatile uint8_t led_state=0;

void timerCommFn(void) {
  comm_flag=1;
}


int main(int argc, char* argv[]) {
  digio_configurePin(13, Output);
  
  struct Timer* timer_comm=Timer_create(10, timerCommFn, 0);
  Timer_start(timer_comm);

  Timer_init();

  HermesComm_init(O_NRF24L01|O_UART);

  
  while(1) {
    system_status.idle_cycles++;
    if(comm_flag) {
      HermesComm_handle();
      comm_flag=0;
    }
  }

  return 0;
}
