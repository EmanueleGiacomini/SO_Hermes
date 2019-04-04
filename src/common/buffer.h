/**
 * buffer.h
 **/

#pragma once
#include <stdint.h>

void buffer_insert(uint8_t* buf, uint16_t* idx, uint16_t size, uint16_t c);

uint8_t buffer_read(uint8_t* buf, uint16_t* idx, uint16_t size);
