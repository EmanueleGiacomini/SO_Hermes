/**
 * joint_test.c
 **/

#include "serial.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "packet_handler.h"
#include "print_packets.h"
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

char buf[256];
PacketHandler _phandler;
PacketHandler* ph=&_phandler;

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

MotorStatusPacket motor_status={
  {
    .id=ID_MOTOR_STATUS_PACKET,
    .size=sizeof(motor_status),
    .seq=0,
    .dest_addr=0,
    .src_addr=0,
    .checksum=0,
  },
  .encoder_ticks=0,
  .desired_speed=0,
  .measured_speed=0,
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
  .rx_packets=0,
  .rx_errors=0,
  .tx_packets=0,
  .idle_cycles=0,
};

void recvFn(PacketHeader* recvp, void* _args);

PacketOperation motor_control_op={
  .id=ID_MOTOR_CONTROL_PACKET,
  .size=sizeof(motor_control),
  .on_receive_fn=recvFn,
  .args=(void*)&motor_control
};


PacketOperation motor_status_op={
  .id=ID_MOTOR_STATUS_PACKET,
  .size=sizeof(motor_status),
  .on_receive_fn=recvFn,
  .args=(void*)&motor_status
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
  PrintPacket(dest, buf);
  printf("\33[2K");
  printf("%s\r", buf);
  fflush(stdout);
}

int serialfd;
sem_t serial_sem;
pthread_t write_thread;
uint8_t wthread_term_flag=0;

pthread_t read_thread;

void threadFn(void) {
  printf("[WThread] starting...\n");
  motor_control.mode=1;
  motor_control.speed=20;
  while(!wthread_term_flag) {
    //++motor_control.speed;
    //if(motor_control.speed>80) {
    //  motor_control.speed=-80;
    //}
    PacketHandler_sendPacket(ph, (PacketHeader*)&motor_control);
    while(PacketHandler_txSize(ph)) {
      uint8_t c=PacketHandler_writeByte(ph);
      write(serialfd, &c, 1);
    }
    usleep(200000);
  }
  printf("[WThread] Exiting...\n");
  pthread_exit(NULL);
  return;
}

void readThreadFn() {
  printf("[RThread] starting...\n");
  uint8_t c;
  while(!wthread_term_flag) {
    int n=read(serialfd, &c, 1);
    if(n) {
      PacketStatus status=PacketHandler_readByte(ph, c);
    }
  }
  printf("[RThread] Exiting...\n");
  pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
  serialfd=setupSerial("/dev/ttyACM0", 57600);
  if(serialfd<0) {
    printf("Error while setting serial...\n");
    return -1;
  }
  PacketHandler_init(ph);
  PacketHandler_addOperation(ph, &motor_status_op);
  PacketHandler_addOperation(ph, &motor_control_op);
  PacketHandler_addOperation(ph, &system_status_op);

  if(sem_init(&serial_sem, 0, 1)<0) {
    printf("Error in sem_init...\n");
    return -1;
  }
  printf("serial semaphore initialized...\n");

  if(pthread_create(&write_thread, NULL, (void*)&threadFn, NULL)<0) {
    printf("Error in pthread_create...\n");
    return -1;
  }
  printf("write_thread created...\n");
  
  if(pthread_create(&read_thread, NULL, (void*)&readThreadFn, NULL)<0) {
    printf("Error in pthread_create...\n");
    return -1;
  }
  printf("read_thread created...\n");
  
  
  sleep(100);
  wthread_term_flag=1;
  if(pthread_join(write_thread, NULL)<0) {
    printf("Error in pthread_join...\n");
    return -1;
  }
  printf("Exiting...\n");  
  return 0;
}

