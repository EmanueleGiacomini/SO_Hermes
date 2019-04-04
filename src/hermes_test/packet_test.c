/**
 * packet_test.c
 **/
#include <stdint.h>
#include "hermes_packets.h"
#include "packet_handler.h"
#include "packet_operations.h"
#include "digio.h"
#include "uart.h"
#include "delay.h"

void receiveFn(void) {
  static uint8_t state=0;
  digio_setPin(13, state);
  state=!state;
  return;
}

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
  .speed=133
};

/**
PacketOperation motor_control_packet_ops={
  .id=ID_MOTOR_CONTROL_PACKET,
  .rx_buf=(uint8_t*)&motor_control_packet,
  .rx_size=sizeof(MotorControlPacket),
  .rx_start=0,
  .rx_end=0,
  .on_receive_fn=receiveFn,
  .args=0,
};
**/

void flushBuffer(struct Uart* u, PacketHandler* h) {
  uint8_t tx_size=h->tx_size;
  for(int i=0;i<tx_size;++i) {
    uint8_t c=PacketHandler_writeByte(h);
    Uart_write(u, c);
  }
}

PacketHandler packet_handler;

int main(int argc, char** argv) {
  struct Uart* u1 = Uart_init(115200);
  PacketHandler_init(&packet_handler);
  while(1) {
    motor_control_packet.speed+=10;
    PacketHandler_sendPacket(&packet_handler, (PacketHandler*)&motor_control_packet);
    flushBuffer(u1, &packet_handler);
    delay(500);
  }
  
  /**
  PacketHandler_addOperation(&packet_handler, &motor_control_packet_ops);

  while(1) {
    uint8_t read_byte=Uart_read(u1);
    PacketHandler_readByte(&packet_handler, read_byte);
  }
  **/
  return 0;
}
