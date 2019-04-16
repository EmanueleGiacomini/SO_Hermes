/**
 * uart_packet.c
 **/

#include "serial.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "packet_handler.h"
#include "print_packets.h"

char buf[256];

void recvFn(PacketHeader* recvp, void* _args);

MotorControlPacket motor_control={
  {
    .id=ID_MOTOR_CONTROL_PACKET,
    .size=sizeof(motor_control),
    .seq=0,
    .dest_addr=0,
    .src_addr=0,
    .checksum=0,
  },
  .mode=0,
  .speed=0
};

PacketOperation motor_control_op={
  .id=ID_MOTOR_CONTROL_PACKET,
  .size=sizeof(motor_control),
  .on_receive_fn=recvFn,
  .args=(void*)&motor_control
};

void recvFn(PacketHeader* recvp, void* _args) {
  PacketHeader* dest=(PacketHeader*)_args;
  memcpy(dest, recvp, dest->size);
  PrintPacket(dest, buf);
  printf("\33[2K");
  printf("%s\r", buf);
  fflush(stdout);
}

PacketHandler phandler;

int main(int argc, char** argv) {
  int serialfd=setupSerial("/dev/ttyACM0", 57600);
  if(serialfd<0) {
    printf("Error while setting serial...\n");
    return -1;
  }

  PacketHandler_init(&phandler);
  PacketHandler_addOperation(&phandler, &motor_control_op);
  uint8_t c;
  
  while(1) {
    if(read(serialfd, &c, 1)<0){
      printf("Error in read\n");
      return -1;
    }
    int ret=PacketHandler_readByte(&phandler, c);
    if(ret==ChecksumError) {
      printf("\nChecksumError!\n");
    }
    if(ret==WrongSize) {
      printf("\nWrongSize!\n");
    }
    if(ret==UnknownType) {
      printf("\nUnknownType!\n");
    }
  }
                  
  return 0;
}

