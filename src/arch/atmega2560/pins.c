/**
 * pins.cpp
 **/

#include "pins.h"
#include <avr/io.h>

Pin _pins[] = {
  {// 0
    .r_ddr=&DDRE,
    .r_port=&PORTE,
    .r_pin=&PINE,
    .pin=0,
  },
  {// 1
    .r_ddr=&DDRE,
    .r_port=&PORTE,
    .r_pin=&PINE,
    .pin=1,
  },
  {// 2
    .r_ddr=&DDRE,
    .r_port=&PORTE,
    .r_pin=&PINE,
    .pin=4,
  },
  {// 3
    .r_ddr=&DDRE,
    .r_port=&PORTE,
    .r_pin=&PINE,
    .pin=5,
  },
  {// 4
    .r_ddr=&DDRG,
    .r_port=&PORTG,
    .r_pin=&PING,
    .pin=5,
  },
  {// 5
    .r_ddr=&DDRE,
    .r_port=&PORTE,
    .r_pin=&PINE,
    .pin=3,
  },
  {// 6
    .r_ddr=&DDRH,
    .r_port=&PORTH,
    .r_pin=&PINH,
    .pin=3,
  },
  {// 7
    .r_ddr=&DDRH,
    .r_port=&PORTH,
    .r_pin=&PINH,
    .pin=4,
  },
  {// 8
    .r_ddr=&DDRH,
    .r_port=&PORTH,
    .r_pin=&PINH,
    .pin=5,
  },
  {// 9
    .r_ddr=&DDRH,
    .r_port=&PORTH,
    .r_pin=&PINH,
    .pin=6,
  },
  {// 10
    .r_ddr=&DDRB,
    .r_port=&PORTB,
    .r_pin=&PINB,
    .pin=4,
  },
  {// 11
    .r_ddr=&DDRB,
    .r_port=&PORTB,
    .r_pin=&PINB,
    .pin=5,
  },
  {// 12
    .r_ddr=&DDRB,
    .r_port=&PORTB,
    .r_pin=&PINB,
    .pin=6,
  },
  {// 13
    .r_ddr=&DDRB,
    .r_port=&PORTB,
    .r_pin=&PINB,
    .pin=7,
  }
};
