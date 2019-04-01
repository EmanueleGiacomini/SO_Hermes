/**
 * uart_test.c
 **/

#include "uart.h"
#include "delay.h"
#include "digio.h"

int main(int argc, char** argv) {
  struct Uart* uart=Uart_init(115200);
  digio_configurePin(13, Output);
  digio_setPin(13, Low);
  while(1) {
    while(Uart_available(uart)) {
      uint8_t c = Uart_read(uart);
      Uart_write(uart, c);
    }    
  }
  return 0;
}
