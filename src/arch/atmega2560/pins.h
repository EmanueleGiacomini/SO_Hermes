/**
 * pins.h
 **/

#include <avr/io.h>
#include <stdint.h>

#define PIN_NUMBER 13

typedef struct {
  volatile uint8_t* r_ddr;
  volatile uint8_t* r_port;
  volatile uint8_t* r_pin;
  uint8_t pin;
}Pin;

extern Pin _pins[PIN_NUMBER];
