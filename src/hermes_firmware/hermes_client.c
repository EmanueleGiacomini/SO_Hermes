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
    .exp_size=sizeof(MotorControlPacket),
    .rx_buf=(uint8_t*)&motor_control_packet,
    .rx_size=sizeof(MotorControlPacket),
    .rx_start=0,
    .rx_end=0,
    .on_receive_fn=receiveFn,
    .args=&motor_control_packet,
  };
  
  PacketHandler _ph;
  PacketHandler* ph=&_ph;
  PacketHandler_init(ph);
  PacketHandler_addOperation(ph, &motor_control_packet_ops);
  uint8_t c = 0x00;
  uint8_t cnt = 0;
  //main loop
  while(1) {
    //rx
    uint8_t pipe = 0;
    if(nrf24l01_readready(&pipe)) { //if data is ready
      cnt++;
      //read buffer
      //nrf24l01_read(bufferin);
      
      nrf24l01_read(&c);
      PacketHandler_readByte(ph, c);
      
      if(cnt%10 == 0) {
        Uart_write(uart, motor_control_packet.speed);
        Uart_write(uart, ' ');
        Uart_write(uart, recv_packets);
        Uart_write(uart, '\n');
      }
      //Uart_write(uart, motor_control_packet.speed);
      
    }
    _delay_ms(10);
  }
  
  return 0;
  
}



/* old test
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
*/
