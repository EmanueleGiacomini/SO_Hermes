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

static uint16_t __debug_succesful_rx_packets=0;
static uint16_t __debug_error_rx_packets=0;

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

MotorStatusPacket motor_status={
  {
    .id=ID_MOTOR_STATUS_PACKET,
    .size=sizeof(motor_status),
    .seq=0,
    .dest_addr=0,
    .src_addr=0,
    .checksum=0,
  },
};

PacketOperation motor_status_op={
  .id=ID_MOTOR_STATUS_PACKET,
  .size=sizeof(motor_status),
  .on_receive_fn=recvFn,
  .args=(void*)&motor_status
};

SystemStatusPacket system_status={
  {
    .id=ID_SYSTEM_STATUS_PACKET,
    .size=sizeof(system_status),
    .seq=0,
    .dest_addr=0,
    .src_addr=0,
    .checksum=0,
  },
};

PacketOperation system_status_op={
  .id=ID_SYSTEM_STATUS_PACKET,
  .size=sizeof(system_status),
  .on_receive_fn=recvFn,
  .args=(void*)&system_status
};

void recvFn(PacketHeader* recvp, void* _args) {
  PacketHeader* dest=(PacketHeader*)_args;
  memcpy(dest, recvp, dest->size);
  __debug_succesful_rx_packets++;
  PrintPacket(dest, buf);
  printf("\33[2K");
  printf("%s\r", buf);
  fflush(stdout);
}

PacketHandler phandler;

uint8_t __debug_ibctr=0;// incoming byte counter
uint8_t __debug_ipctr=0;// incoming packet counter

int main(int argc, char* argv[]) {
  int serialfd=setupSerial(argv[1], 57600);
  if(serialfd<0) {
    printf("Error while setting serial... port:%s\n", argv[1]);
    return -1;
  }

  PacketHandler_init(&phandler);
  PacketHandler_addOperation(&phandler, &motor_control_op);
  PacketHandler_addOperation(&phandler, &motor_status_op);
  PacketHandler_addOperation(&phandler, &system_status_op);
  
  while(1) {
    uint8_t c;
    uint8_t packet_complete=0;
    while(packet_complete==0) {
      int n=read(serialfd, &c, 1);
      if(n) {
        fflush(stdout);
        PacketStatus status=PacketHandler_readByte(&phandler, c);
        if(status<0) {
          //printf("\nError at %d\n", __debug_succesful_rx_packets);
          __debug_error_rx_packets++;
          //printf("error no: %d\n", status);
        }
        packet_complete=(status==ChecksumSuccess);
      }              
    }    
  }
                  
  return 0;
}

