/**
 * packet_test.c
 **/
#include <stdint.h>
#include "hermes_packets.h"
#include "packet_handler.h"
#include "packet_operations.h"
#include "digio.h"

void receiveFn(void) {
  static uint8_t state=0;
  digio_setPin(13, state);
  state=!state;
  return;
}

MotorControlPacket motor_control_packet;
PacketOperation motor_control_packet_ops={
  .id=ID_MOTOR_CONTROL_PACKET,
  .rx_buf=(uint8_t*)&motor_control_packet,
  .rx_size=sizeof(MotorControlPacket),
  .rx_start=0,
  .rx_end=0,
  .on_receive_fn=&receiveFn,
  .args=0,
};

PacketHandler packet_handler;

int main(int argc, char** argv) {
  PacketHandler_init(&packet_handler);
  PacketHandler_addOperation(&packet_handler, &motor_control_packet_ops);

  while(1) {
    uint8_t read_byte=Uart_read(packet_handler.uart);
    PacketHandler_readByte(&packet_handler, read_byte);
  }

  PacketHandler_sendPacket(&packet_handler,(PacketHeader*)&motor_control_packet);

  while(ci spno byte da trasmettere) {
    uint8_t c = PacketHandler_writeByte(&packet_handler);
    Uart_write(c);
  }
  
  return 0;
}
