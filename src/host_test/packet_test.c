/**
 * packet_test.c
 **/

#include "hermes_packets.h"
#include "packet_handler.h"
#include <stdio.h>
#include <stdlib.h>
#include "serial.h"
#include <unistd.h>
#include <stdint.h>

void receiveFn(void);

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
  .speed=255
};
PacketOperation motor_control_packet_ops={
  .id=ID_MOTOR_CONTROL_PACKET,
  .rx_buf=(uint8_t*)&motor_control_packet,
  .rx_size=sizeof(MotorControlPacket),
  .rx_start=0,
  .rx_end=0,
  .on_receive_fn=receiveFn,
  .args=0,
};

void receiveFn(void) {
  printf("%d\n", motor_control_packet.speed);
}

PacketHandler packet_handler;

void _flushBuffer(int fd, uint8_t* buf, uint8_t start, uint8_t size) {
  for(int i=0;i<size;++i) {
    uint8_t c=buf[start+i];
    write(fd, &c, 1);
  }
}

void printPacket() {

}

int main(int argc, char** argv) {
  printf("Starting program\n");
  int serial_fd=setupSerial("/dev/ttyACM0", 57600);
  if(serial_fd<0) {
    printf("Could not open serial...\n");
    return 1;
  } 
  

  PacketHandler_init(&packet_handler);
  PacketHandler_addOperation(&packet_handler, &motor_control_packet_ops);
  /**
  while(1) {
    printf("Sending packet...\n");
    PacketHandler_sendPacket(&packet_handler, (PacketHeader*)&motor_control_packet);
    _flushBuffer(serial_fd, packet_handler.tx_buffer, packet_handler.tx_start, packet_handler.tx_size);
    sleep(1);
  }
  **/
  uint8_t c;
  while(1) {
    read(serial_fd, (void*)&c, (size_t)1);
    PacketHandler_readByte(&packet_handler, c);
  }
  
  return 0;
}
