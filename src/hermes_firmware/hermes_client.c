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
    
    HermesComm_handle();
    delay(10);
    
    if(HermesComm_readPacket((PacketHeader*)&motor_control)==Success) {
      HermesComm_sendPacket((PacketHeader*)&motor_control, O_UART);
    }
    HermesComm_sendPacket((PacketHeader*)&motor_control, O_UART);
    motor_control.h.seq+=1;
    motor_control.speed+=1;
    delay(10);
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
>>>>>>> b02260c209a7563e3111fb553e0eb3d1bf79553f

  }
  
  return 0;
}
*/
