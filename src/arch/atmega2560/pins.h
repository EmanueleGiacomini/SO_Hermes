/**
 * pins.h
 **/

#include <stdint.h>

typedef struct{
  volatile uint8_t* r_ddr;
  volatile uint8_t* r_port;
  volatile uint8_t* r_pin;
  uint8_t pin;
}Pin;

extern Pin _pins[];
