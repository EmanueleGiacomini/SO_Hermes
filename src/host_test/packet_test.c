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

void receiveFn(PacketHeader* p, void* args) {
  ++recv_packets;
  memcpy((PacketHeader*)args, p, p->size);
  Packet_print(p);
  fflush(stdout);
}

void _flushBuffer(int fd, PacketHandler* ph) {
  //printf("#--FB--#####################\n");
  uint8_t sendb=PacketHandler_txSize(ph);
  //printf("Txsize: %d\n", sendb);
  for(int i=0;i<sendb;++i) {
    uint8_t c=PacketHandler_writeByte(ph);
    //printf("%x",c);
    write(fd, &c, 1);
  }
  //printf("\n");
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
    .size=sizeof(motor_control_packet),
    .on_receive_fn=0,
    .args=0,
    .on_receive_fn=receiveFn,
    .args=(void*)&motor_control_packet,
  };
  
  PacketHandler _ph;
  PacketHandler* ph=&_ph;
  PacketHandler_init(ph);
  PacketHandler_addOperation(ph, &motor_control_packet_ops);
  printf("[Receiver] Starting...\n");
  close(pipefd[1]);
  uint8_t c;
  uint8_t ctr=0;
  int8_t ret=0;
  while(1) {
    if(read(pipefd[0], &c, 1)==-1) {
      printf("Error during read...\n");
      break;
    }
    //printf("%x", c);
    ctr++;
    if(ctr%(sizeof(MotorControlPacket)+2)==0) {
      //printf("\n");
    }
    
    ret=PacketHandler_readByte(ph, c);
    if(ret) {
      printf("ERROR %d while reading packet...\n", ret);
    }
    
  }
  printf("[Receiver] Closing...\n");
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
    usleep(10000);
  }
  printnf("[Transmitter] Closing...\n");
  close(pipefd[1]);
  return;
}


int main(int argc, char** argv) {
  if(pipe(pipefd)) {
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
