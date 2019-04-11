/**
 * hermes_comm.c
 **/

#include "hermes_comm.h"
#include "uart.h"

#define PACKET_BUFFER_SIZE 8

void HermesComm_receivePacketFn(PacketHeader*, void*);

#define COPY 0x1
#define TX_UART 0x2
#define TX_NRF 0x4

typedef struct {
  PacketHeader* buffer;
  uint8_t operations;
}HandlePacketFn;

#ifdef _CLIENT

MotorControlPacket motor_control_packet_buffer[PACKET_BUFFER_SIZE];

HandlePacketFn motor_control_args ={
  .buffer=motor_control_packet_buffer,
  .operations=COPY;
};

PacketOperation motor_control_packet_op={
  .id=ID_MOTOR_CONTROL_PACKET,
  .size=sizeof(MotorControlPacket),
  .on_receive_fn=HermesComm_receivePacketFn,
  .args=(void*)&motor_control_args
};

#elseif _RELAY

MotorControlPacket motor_control_packet_buffer[PACKET_BUFFER_SIZE];
MotorStatusPacket motor_status_packet_buffer[PACKET_BUFFER_SIZE];

HandlePacketFn motor_control_args ={
  .buffer=motor_control_packet_buffer,
  .operations=TX_NRF,
};

HandlePacketFn motor_status_args ={
  .buffer=motor_status_packet_buffer,
  .operations=TX_UART,
};


PacketOperation motor_control_packet_op={
  .id=ID_MOTOR_CONTROL_PACKET,
  .size=sizeof(MotorControlPacket),
  .on_receive_fn=HermesComm_receivePacketFn,
  .args=(void*)&motor_control_args
};

PacketOperation motor_status_packet_op={
  .id=ID_MOTOR_STATUS_PACKET,
  .size=sizeof(MotorStatusPacket),
  .on_receive_fn=0,
  .args=(void*)&motor_status_args,
};

#endif



static uint8_t active_interfaces=0;

struct Uart* uart_1;

PacketHandler uart_handler;
PacketHandler nrf_handler;

void HermesComm_init(uint8_t interface) {
  active_interfaces=interface;
  if((interface>>O_UART)&0x1) {
    PacketHandler_init(&uart_handler);
#ifdef _CLIENT
    PacketHandler_addOperation(&uart_handler, &motor_control_packet_op);
#elseif _RELAY
    PacketHandler_addOperation(&uart_handler, &motor_control_packet_op);
    PacketHandler_addOperation(&uart_handler, &motor_status_packet_op);
#endif
    
    // TODO init UART
    uart_1=Uart_init(115200);
  }
  if((interface>>O_NRF24L01)&0x1) {
    PacketHandler_init(&nrf_handler);
    // TODO init NRF24L01
  }
}

PacketStatus HermesComm_sendPacket(PacketHeader* h, uint8_t interface) {
  if((interface>>O_UART)&0x1) {
    // TODO transmit through UART
  } 
  if((interface>>O_NRF24L01)&0x1) {
    // TODO transmit through NRF24L01
  }
}

PacketStatus HermesComm_readPacket(PacketHeader* h) {
  // read from buffer
  
}

PacketStatus HermesComm_handle(void) {
  if((active_interfaces>>O_UART)&0x1) {
    // TODO read bytes buffered from UART
  }
  if((active_interfaces>>O_NRF24L01)&0x1) {
    // TODO read bytes buffered from NRF24L01
  }
  return Success;
}

void HermesComm_receivePacketFn(PacketHeader* p, void* _args) {
  
  return;
}
