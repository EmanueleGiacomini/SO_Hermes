/**
 * print_packets.c
 **/

#include "print_packets.h"
#include <stdio.h>

uint16_t PrintHeader(PacketHeader* h, char* buf) {
  return sprintf(buf, "[id:%d s:%d seq:%d dest:%d src:%d cs:%d]",
                 h->id,
                 h->size,
                 h->seq,
                 h->dest_addr,
                 h->src_addr,
                 h->checksum);
}

uint16_t MotorControlPacket_print(PacketHeader* h, char* buf) {
  MotorControlPacket* p = h;
  uint16_t h_chars=PrintHeader(h, buf); 
  return sprintf(buf+h_chars, "[mode:%d speed:%d]",
                 p->mode,
                 p->speed);
}

uint16_t MotorStatusPacket_print(PacketHeader* h, char* buf) {
  MotorStatusPacket* p = h;
  uint16_t h_chars=PrintHeader(h, buf); 
  return sprintf(buf+h_chars, "[eticks:%d ds:%d ms:%d]",
                 p->encoder_ticks,
                 p->desired_speed,
                 p->measured_speed);
}

uint16_t SystemStatusPacket_print(PacketHeader* h, char* buf) {
  SystemStatusPacket* p = h;
  uint16_t h_chars=PrintHeader(h, buf); 
  return sprintf(buf+h_chars, "[rxp:%d rxe:%d txp:%d idle:%d]",
                 p->rx_packets,
                 p->rx_errors,
                 p->tx_packets,
                 p->idle_cycles);
}

typedef uint16_t (*PrintPacketFn)(PacketHeader*, char*);

typedef struct {
  PrintPacketFn print_fn;
}PrintPacketOps;

static PrintPacketOps print_packet_ops[MAX_PACKET_TYPE] = {
  {//0
    .print_fn=MotorControlPacket_print,
  },
  {//1
    .print_fn=MotorStatusPacket_print,
  },
  {//2
    .print_fn=0,
  },
  {//3
    .print_fn=SystemStatusPacket_print,
  },
};

uint16_t PrintPacket(PacketHeader* h, char* buf) {
  PrintPacketFn fn=print_packet_ops[h->id].print_fn;
  return (*fn)(h, buf);
}
