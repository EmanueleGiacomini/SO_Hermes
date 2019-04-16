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
#include <util/delay.h>

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
  
  while(1) {
    HermesComm_handle();
    //motor_control.speed++;
    //HermesComm_sendPacket(&motor_control.h, O_UART);
    _delay_ms(10);
  }
  
  return 0;
}


/*
static int recv_packets=0;

void receiveFn(PacketHeader* p) {
  ++recv_packets;
  //Packet_print(p);
  fflush(stdout);
}

int main(int argc, char* argv[]) {
  struct Uart* uart=Uart_init(115200);

  //init nrf24l01
  nrf24l01_init();

  //init interrupt
  sei();
  
  MotorControlPacket motor_control_packet = {
    {
      .id=ID_MOTOR_CONTROL_PACKET,
      .size=sizeof(MotorControlPacket),
      .seq=0,
      .dest_addr=0x0A,
      .src_addr=0xDE,
      .checksum=0xBE
    },
    .mode=0,
    .speed=0
  };

  PacketOperation motor_control_packet_ops={
    .id=ID_MOTOR_CONTROL_PACKET,
    .size=sizeof(MotorControlPacket),
    .on_receive_fn=receiveFn,
    .args=&motor_control_packet,
  };

  
  PacketHandler _ph;
  PacketHandler* ph=&_ph;
  PacketHandler_init(ph);
  PacketHandler_addOperation(ph, &motor_control_packet_ops);

  uint8_t rx_buf[NRF24L01_PAYLOAD];
  uint8_t size = 0, idx = 0, i = 0;  
  
  while(1) {
    uint8_t pipe = 0;
    if(nrf24l01_readready(&pipe)) { //if data is ready
      
      nrf24l01_read(rx_buf);
      
      // Size of the packet
      if(rx_buf[0] == 0xAA && rx_buf[1] == 0x55) size = rx_buf[3];
        
      for(i=0; i<NRF24L01_PAYLOAD; ++i) {
        if(idx == size+3) {
          // A packet has been received
          Uart_write(uart, motor_control_packet.speed);
          Uart_write(uart, ' ');
          Uart_write(uart, recv_packets);
          Uart_write(uart, '\n');
          size = 0;
          idx = 0;
          break;
        }
        PacketHandler_readByte(ph, rx_buf[i]);
        ++idx;
      }
      
    }
    _delay_ms(150);
  }
  
  return 0;
  
}

*/

/*
// old test
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


  //uint8_t test[NRF24L01_PAYLOAD] = "ciao so io :-)!!";
  uint8_t test[NRF24L01_PAYLOAD] = "negretto amico!\n";

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

  uint8_t cnt = 0;
  //main loop
  while(1) {

    //rx
    uint8_t pipe = 0;
    if(nrf24l01_readready(&pipe)) { //if data is ready
      cnt++;
      //read buffer
      nrf24l01_read(bufferin);
    
      for(i=0; i<sizeof(bufferin); i++) {
        Uart_write(uart, bufferin[i]);
      }
      Uart_write(uart, ' ');
      Uart_write(uart, cnt);
      
    }
    _delay_ms(10);
  }
  
  return 0;
}
*/
