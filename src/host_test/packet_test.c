/**
 * packet_test.c
 **/

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "hermes_packets.h"
#include "packet_handler.h"
#include "serial.h"

void Packet_print(PacketHeader* p) {
  printf("{[id:%d s:%d seq:%d dest:%d src:%d checksum:%d] mode:%d speed:%d}\r",
         p->id,
         p->size,
         p->seq,
         p->dest_addr,
         p->src_addr,
         p->checksum,
         ((MotorControlPacket*)p)->mode,
         ((MotorControlPacket*)p)->speed);
}

static int recv_packets=0;

void receiveFn(PacketHeader* p) {
  ++recv_packets;
  Packet_print(p);
  fflush(stdout);
}

void _flushBuffer(int fd, PacketHandler* ph) {
  uint16_t bytes_to_write=ph->tx_size;
  for(int i=0;i<bytes_to_write;++i) {
    uint8_t c=buffer_read(ph->tx_buffer, &ph->tx_start, PACKET_SIZE_MAX);
    write(fd, &c, 1);
    --ph->tx_size;
  }
}


int pipefd[2];


void receiverFn(void) {

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
  printf("[Receiver] Starting...\n");
  close(pipefd[1]);
  uint8_t c;
  uint8_t ctr=0;
  while(1) {
    if(read(pipefd[0], &c, 1)!=1) {
      printf("Error during read...\n");
      break;
    }
    //printf("%x", c);
    ctr++;
    //if(ctr%(sizeof(MotorControlPacket)+2)==0) {
      //printf("\n");
      //}
    if(PacketHandler_readByte(ph, c)) {
      printf("ERROR while reading packet...\n");
    }
  }
  close(pipefd[0]);
  return;
}

void transmitterFn(void) {
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

  PacketHandler _ph;
  PacketHandler* ph=&_ph;
  PacketHandler_init(ph);
  printf("[Transmitter] Starting...\n");
  close(pipefd[0]);
  while(1) {
    motor_control_packet.speed++;
    motor_control_packet.mode=(motor_control_packet.mode+1)%2;
    PacketHandler_sendPacket(ph, (PacketHeader*)&motor_control_packet);
    _flushBuffer(pipefd[1], ph);
    usleep(100000);
  }
  close(pipefd[1]);
  return;
}


int main(int argc, char** argv) {
  if(pipe2(pipefd, 0)) {
    printf("Error while executing pipe2...exiting\n");
    return 1;
  }  
  pid_t pid;
  pid=fork();
  if(pid==0) {
    receiverFn();
  } else {
    transmitterFn();
    exit(0);
  }
  int retval;
  wait(&retval);
  printf("Closing test\n");
  return;
}
