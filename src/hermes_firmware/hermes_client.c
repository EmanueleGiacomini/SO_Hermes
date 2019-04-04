#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "uart.h"
#include "delay.h"
#include "digio.h"
#include "spi.h"
#include "hermes_nRF24L01.h"


//main here
int main(void) {

  struct Uart* uart=Uart_init(115200);

  uint8_t i = 0;

  //nrf24l01 variables
  uint8_t bufferout[NRF24L01_PAYLOAD];
  uint8_t bufferin[NRF24L01_PAYLOAD];


  //init nrf24l01
  nrf24l01_init();

  //init interrupt
  sei();


  uint8_t test[NRF24L01_PAYLOAD] = "ciao so io :-)!!";

  //setup buffer
  for(i=0; i<sizeof(bufferout); i++)
    bufferout[i] = test[i];
  for(i=0; i<sizeof(bufferin); i++)
    bufferin[i] = 0;



  //sending buffer addresses
  uint8_t sendpipe = 0;
  uint8_t addrtx0[NRF24L01_ADDRSIZE] = NRF24L01_ADDRP0;
  uint8_t addrtx1[NRF24L01_ADDRSIZE] = NRF24L01_ADDRP1;
  uint8_t addrtx2[NRF24L01_ADDRSIZE] = NRF24L01_ADDRP2;
  uint8_t addrtx3[NRF24L01_ADDRSIZE] = NRF24L01_ADDRP3;
  uint8_t addrtx4[NRF24L01_ADDRSIZE] = NRF24L01_ADDRP4;
  uint8_t addrtx5[NRF24L01_ADDRSIZE] = NRF24L01_ADDRP5;


  //main loop
  while(1) {

    //rx
    uint8_t pipe = 0;
    if(nrf24l01_readready(&pipe)) { //if data is ready

      //read buffer
      nrf24l01_read(bufferin);

      for(i=0; i<sizeof(bufferin); i++) {
        Uart_write(uart, bufferin[i]);
      }
      
    }
    _delay_ms(10);
  }
  
  return 0;
}
