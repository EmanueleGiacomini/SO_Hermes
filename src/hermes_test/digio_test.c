/**
 * digio_test.c
 **/

#include "digio.h"
#include "delay.h"

int main(int argc, char** argv) {
  digio_configurePin(13, Output);
  while(1) {    
    digio_setPin(13, High);
    delay(1000);
    digio_setPin(13, Low);
    delay(1000);
  }
  return 0;
}
