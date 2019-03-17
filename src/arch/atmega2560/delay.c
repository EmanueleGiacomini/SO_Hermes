/**
 * delay.c
 **/

#include "delay.h"
#include <util/delay.h>

/**
 * Stop the program for ms milliseconds
 **/
void delay(uint16_t ms) {
  for(int i=0;i<ms;++i)
    _delay_ms(1);
}

