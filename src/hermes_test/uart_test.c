
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


//role definitions
#define ROLETX 1
#define ROLERX 0


#define LEDOUT_DDR DDRB
#define LEDOUT_PORT PORTB
#define LEDOUT PB6
#define LEDOUTPAUSE 300

//main here
int main(void) {
  
  struct Uart* uart=Uart_init(115200);
  
	uint8_t txrxrole = 0; // 1 transmitter 0 receiver
	uint8_t i = 0;

	//nrf24l01 variables
	uint8_t bufferout[NRF24L01_PAYLOAD];
	uint8_t bufferin[NRF24L01_PAYLOAD];

	//setup port
	LEDOUT_DDR |= (1<<LEDOUT); //output
	LEDOUT_PORT &= ~(1<<LEDOUT); //off


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
	for(;;) {
		if(txrxrole == ROLETX) {
				if(sendpipe == 0) {
					//set tx address for pipe 0
					nrf24l01_settxaddr(addrtx0);
				} else if(sendpipe == 1) {
					//set tx address for pipe 1
					nrf24l01_settxaddr(addrtx1);
				} else if(sendpipe == 2) {
					//set tx address for pipe 2
					nrf24l01_settxaddr(addrtx2);
				} else if(sendpipe == 3) {
					//set tx address for pipe 3
					nrf24l01_settxaddr(addrtx3);
				} else if(sendpipe == 4) {
					//set tx address for pipe 4
					nrf24l01_settxaddr(addrtx4);
				} else if(sendpipe == 5) {
					//set tx address for pipe 5
					nrf24l01_settxaddr(addrtx5);
				}

				//write buffer
				uint8_t writeret = nrf24l01_write(bufferout);



				sendpipe++;
				sendpipe%=6;

				//blink led for output data
				LEDOUT_PORT |= (1<<LEDOUT); _delay_ms(LEDOUTPAUSE);
				LEDOUT_PORT &= ~(1<<LEDOUT); _delay_ms(LEDOUTPAUSE);

			

			#if DEBUGENABLED == 1
			_delay_ms(1000);
			#endif
		} else if(txrxrole == ROLERX) {

			//rx
			uint8_t pipe = 0;
			if(nrf24l01_readready(&pipe)) { //if data is ready


				//read buffer
				nrf24l01_read(bufferin);
        int samecheck = 1;
        for(i=0; i<sizeof(bufferin); i++) {
					if(bufferin[i] != bufferout[i])
						samecheck = 0;
					Uart_write(uart, bufferin[i]);
				}
        if(samecheck) Uart_write(uart, 0x55);



				//blink led for input data
				LEDOUT_PORT |= (1<<LEDOUT); _delay_ms(LEDOUTPAUSE);
				LEDOUT_PORT &= ~(1<<LEDOUT); _delay_ms(LEDOUTPAUSE);
			}
			_delay_ms(10);
    }
  }
}
