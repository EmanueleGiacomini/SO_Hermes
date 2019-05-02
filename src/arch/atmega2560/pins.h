/**
 * pins.h
 **/

#include <stdint.h>

typedef struct{
  // digio
  volatile uint8_t* r_ddr;
  volatile uint8_t* r_port;
  volatile uint8_t* r_pin;
  uint8_t pin;
  // pwm
  volatile uint8_t* tccr;
  volatile uint8_t* ocr;
  uint8_t mask;
  
}Pin;

extern Pin _pins[];
