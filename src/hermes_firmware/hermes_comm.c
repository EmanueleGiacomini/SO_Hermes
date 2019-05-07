/**
 * hermes_comm.c
 **/

#include "hermes_comm.h"
#include "hermes_globals.h"
#include "uart.h"
#include "digio.h"
#include <string.h>

#define PACKET_BUFFER_SIZE 8

void HermesComm_receivePacketFn(PacketHeader*, void*);

static uint8_t buffer_start[MAX_PACKET_TYPE];
static uint8_t buffer_end[MAX_PACKET_TYPE];
static uint16_t buffer_size[MAX_PACKET_TYPE];

static uint16_t global_seq=0;

#define COPY 0x1
#define TX_UART 0x2
#define TX_NRF 0x4

typedef struct {
  void* buffer;
  uint8_t operations;
}HandlePacketFn;


#ifdef _CLIENT
MotorControlPacket motor_control_packet_buffer[PACKET_BUFFER_SIZE];
MotorParamsPacket motor_params_packet_buffer[PACKET_BUFFER_SIZE];
void* packet_buffers[MAX_PACKET_TYPE]={
  (void*)motor_control_packet_buffer,
  0,
  (void*)motor_params_packet_buffer,
};

HandlePacketFn motor_control_args ={
  .buffer=motor_control_packet_buffer,
  .operations=COPY,
  //.operations=TX_UART,
};

HandlePacketFn motor_params_args ={
  .buffer=motor_params_packet_buffer,
  .operations=COPY,
};

PacketOperation motor_control_packet_op={
  .id=ID_MOTOR_CONTROL_PACKET,
  .size=sizeof(MotorControlPacket),
  .on_receive_fn=HermesComm_receivePacketFn,
  .args=(void*)&motor_control_args
};

PacketOperation motor_params_packet_op={
  .id=ID_MOTOR_PARAMS_PACKET,
  .size=sizeof(MotorParamsPacket),
  .on_receive_fn=HermesComm_receivePacketFn,
  .args=(void*)&motor_params_args,
};

#elif _RELAY

void* packet_buffers[MAX_PACKET_TYPE]={};

HandlePacketFn motor_control_args ={
  .buffer=0,
  .operations=TX_NRF,
};

HandlePacketFn motor_status_args ={
  .buffer=0,
  .operations=TX_UART,
};

HandlePacketFn motor_params_args ={
  .buffer=0,
  .operations=TX_NRF,
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
  .on_receive_fn=HermesComm_receivePacketFn,
  .args=(void*)&motor_status_args,
};

PacketOperation motor_params_packet_op={
  .id=ID_MOTOR_PARAMS_PACKET,
  .size=sizeof(MotorParamsPacket),
  .on_receive_fn=HermesComm_receivePacketFn,
  .args=(void*)&motor_params_args,
};


#endif

static uint8_t active_interfaces=0;

struct Uart* uart_1;

PacketHandler uart_handler;
PacketHandler nrf_handler;

// nRF24L01 transmitting pipes
static uint8_t sendpipe;
static uint8_t addrtx0[NRF24L01_ADDRSIZE] = NRF24L01_ADDRP0;
static uint8_t addrtx1[NRF24L01_ADDRSIZE] = NRF24L01_ADDRP1;
static uint8_t addrtx2[NRF24L01_ADDRSIZE] = NRF24L01_ADDRP2;
static uint8_t addrtx3[NRF24L01_ADDRSIZE] = NRF24L01_ADDRP3;
static uint8_t addrtx4[NRF24L01_ADDRSIZE] = NRF24L01_ADDRP4;
static uint8_t addrtx5[NRF24L01_ADDRSIZE] = NRF24L01_ADDRP5;


void HermesComm_init(uint8_t interface) {
  digio_configurePin(2, Output);
  active_interfaces=interface;
  if(interface & O_UART) {
    PacketHandler_init(&uart_handler);
#ifdef _CLIENT
    PacketHandler_addOperation(&uart_handler, &motor_control_packet_op);
#elif _RELAY
    PacketHandler_addOperation(&uart_handler, &motor_control_packet_op);
    PacketHandler_addOperation(&uart_handler, &motor_status_packet_op);
#endif
    
    // TODO init UART
    uart_1=Uart_init(115200);
  }
  if(interface & O_NRF24L01) {
    PacketHandler_init(&nrf_handler);
#ifdef _CLIENT
    PacketHandler_addOperation(&nrf_handler, &motor_control_packet_op);
#elif _RELAY
    PacketHandler_addOperation(&nrf_handler, &motor_control_packet_op);
    PacketHandler_addOperation(&nrf_handler, &motor_status_packet_op);
#endif

    nrf24l01_init(); // Initializing nrf24l01 chip
    sei(); // Enabling use of interrupts
    sendpipe = 0;
    
  }
}

PacketStatus HermesComm_sendPacket(PacketHeader* h, uint8_t interface) {
  cli();
  ++system_status.tx_packets; // statistics
  h->seq=global_seq;
  if(interface & O_UART) {
    PacketHandler* ph=&uart_handler;
    PacketHandler_sendPacket(ph, h);
    while(PacketHandler_txSize(ph)>0) {
      uint8_t c=PacketHandler_writeByte(ph);
      Uart_write(uart_1, c);
    } 
  }
  
  if(interface & O_NRF24L01) {
    digio_setPin(2, 1);
    PacketHandler* ph=&nrf_handler;
    
    // Setting the right pipe address
    switch(sendpipe) {
    case 0:
      nrf24l01_settxaddr(addrtx0);
      break;
    case 1:
      nrf24l01_settxaddr(addrtx1);
      break;
    case 2:
      nrf24l01_settxaddr(addrtx2);
      break;
    case 3:
      nrf24l01_settxaddr(addrtx3);
      break;
    case 4:
      nrf24l01_settxaddr(addrtx4);
      break;
    case 5:
      nrf24l01_settxaddr(addrtx5);
      break;
    default:
      return UnknownType;
    }

    PacketHandler_sendPacket(ph, h);

    uint8_t bytes_to_write=PacketHandler_txSize(ph);
    uint8_t tx_buf[NRF24L01_PAYLOAD];

    // Starting transmission of the packet in blocks of NRF24L01_PAYLOAD bytes
    int i, fixed_size = bytes_to_write - (bytes_to_write%NRF24L01_PAYLOAD);
    
    for(i=0; i<fixed_size; ++i) {
      if(!(i % NRF24L01_PAYLOAD)) {
        nrf24l01_write(tx_buf);
      }
      tx_buf[i % NRF24L01_PAYLOAD] = PacketHandler_writeByte(ph);
    }

    // Sending the rest
    for(; i<bytes_to_write; ++i) {
      tx_buf[i % NRF24L01_PAYLOAD] = PacketHandler_writeByte(ph);
    }

    nrf24l01_write(tx_buf);

    // Using all the pipes to transmit
    sendpipe++;
    sendpipe%=6;
    digio_setPin(2, 0);
  }
  sei();
  global_seq++;
  return Success;
}

PacketStatus HermesComm_readPacket(PacketHeader* h) {
  // read from buffer
  uint8_t pid=h->id;
  if(buffer_size[pid]==0)
    return BufferEmtpy;
  void* pbuf=packet_buffers[pid]+buffer_start[pid]*h->size;
  buffer_start[pid]=(buffer_start[pid]+1)%PACKET_BUFFER_SIZE;
  --buffer_size[pid];
  memcpy((void*)h, pbuf, h->size);
  return Success;
}

PacketStatus HermesComm_handle(void) {
  if(active_interfaces & O_UART) {
    PacketHandler* ph=&uart_handler;
    while(Uart_available(uart_1)) {
      uint8_t c = Uart_read(uart_1);
      if(PacketHandler_readByte(ph, c)<0) {
        ++system_status.rx_errors;// statistics
      }
    }
  }
  if(active_interfaces & O_NRF24L01) {
    PacketHandler* ph=&nrf_handler;
    
    uint8_t rx_buf[NRF24L01_PAYLOAD];
    uint8_t size = 0, idx = 0, i = 0, pipe = 0;
    
    // Check if data is avaiable
    if(nrf24l01_readready(&pipe)) {
      nrf24l01_read(rx_buf); // Read bytes and put in rx_buf
      // Size of the packet
      
      if(rx_buf[0] == 0xAA && rx_buf[1] == 0x55) size = rx_buf[3];

      for(i=0; i<NRF24L01_PAYLOAD; ++i) {
        if(idx == size+3) {
          // A packet has been received
          size = 0;
          idx = 0;
          break;
        }
        if(PacketHandler_readByte(ph, rx_buf[i])<0) {
          //++system_status.rx_errors;// statistics
        }
        ++idx;
      }
    }
  }
  
  return Success;
}

volatile uint8_t __debug_led_state=0;

void HermesComm_receivePacketFn(PacketHeader* p, void* _args) {
  system_status.rx_packets++;// statistics
  digio_setPin(2, __debug_led_state);
  __debug_led_state=!__debug_led_state;  
  
  HandlePacketFn* args=(HandlePacketFn*)_args;
  uint8_t ops=args->operations;
  if(ops&TX_UART) {
    HermesComm_sendPacket(p, O_UART);
  }
  if(ops&TX_NRF) {
    HermesComm_sendPacket(p, O_NRF24L01);
  }
  if(ops&COPY) {
    uint8_t pid=p->id;
    if(buffer_size[pid]<PACKET_BUFFER_SIZE) { // buffer not full
      void* pbuf=(void*)args->buffer+p->size*buffer_end[pid];
      memcpy(pbuf, (void*)p, p->size);
      ++buffer_size[pid];
      buffer_end[pid]=(buffer_end[pid]+1)%PACKET_BUFFER_SIZE;
    }
  }
  return;
}
