/**
 * buffer.c
 **/

#include "buffer.h"

void buffer_insert(uint8_t* buf, uint16_t* idx, uint16_t size, uint16_t c) {
  buf[*idx]=c;
  ++(*idx);
  if(*idx>=size)
    *idx=0;
  return;
}

uint8_t buffer_read(uint8_t* buf, uint16_t* idx, uint16_t size) {
  uint8_t c=buf[*idx];
  ++(*idx);
  if(*idx>=size)
    *idx=0;
  return c;
}
