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
  /**
  PrintPacket(dest, buf);
  printf("\33[2K");
  printf("%s\r", buf);
  fflush(stdout);
  **/
}

PacketHandler phandler;

uint8_t __debug_ibctr=0;// incoming byte counter
uint8_t __debug_ipctr=0;// incoming packet counter

int main(int argc, char** argv) {
  int serialfd=setupSerial("/dev/ttyACM0", 57600);
  if(serialfd<0) {
    printf("Error while setting serial...\n");
    return -1;
  }

  PacketHandler_init(&phandler);
  PacketHandler_addOperation(&phandler, &motor_control_op);
  
  while(1) {
    uint8_t c;
    uint8_t packet_complete=0;
    while(packet_complete==0) {
      int n=read(serialfd, &c, 1);
      if(n) {
        fflush(stdout);
        PacketStatus status=PacketHandler_readByte(&phandler, c);
        if(status<0) {
          printf("error no: %d\n", status);
        }
        packet_complete=(status==ChecksumSuccess);
      }              
    }
    printf("recv packet\n");
    //============= debug
    /**
    printf("%02x", c);
    __debug_ibctr++;
    if(__debug_ibctr%(sizeof(motor_control)+2)==0) {
      __debug_ipctr++;
      printf("\t%d\n", __debug_ipctr);
      if(__debug_ipctr%30==0) {
        //printf("\033[30A");
        printf("\n");
        return 0;
      }
      fflush(stdout);
    }
    **/
    //============= debug
    
  }
                  
  return 0;
}

