/**
   digio.h
**/

#pragma once
#include <stdint.h>

typedef enum {
  Input=0,
  Output=1
}PinMode;

typedef enum {
  Low=0,
  High=1
}PinStatus;

void digio_configurePin(uint8_t , PinMode);
void digio_setPin(uint8_t , PinStatus);
uint8_t digio_readPin(uint8_t);

