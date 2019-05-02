/*
 * 
 * Hermes client, endpoint receiving from relay
 * 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "uart.h"
#include "delay.h"
#include "digio.h"
#include "spi.h"
#include "hermes_nRF24L01.h"

#include <fcntl.h>
#include "hermes_packets.h"
#include "packet_handler.h"
#include "hermes_globals.h"
#include "hermes_comm.h"


int main(int argc, char* argv[]) {
  HermesComm_init(O_NRF24L01|O_UART);
  digio_configurePin(13, Output);
  digio_setPin(13, Low);
  
  while(1) {
    
    PacketStatus res = HermesComm_handle();
    delay(10);

  }
  
  return 0;
}
