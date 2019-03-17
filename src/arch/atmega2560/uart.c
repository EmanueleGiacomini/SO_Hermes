/**
 * uart.h
 *
 * This section handle the low level mechanics of the UART module.
 * Different functions have to be made, in order to send and receive
 * bytes.
 **/
#include "uart.h"
#include <avr/io.h>
#include <avr/interrupt.h>

#define BUF_SIZE 256


typedef struct Uart{
  uint8_t rx_buffer[BUF_SIZE];
  uint8_t rx_start;
  uint8_t rx_end;
  uint8_t rx_size;

  uint8_t tx_buffer[BUF_SIZE];
  uint8_t tx_start;
  uint8_t tx_end;
  uint8_t tx_size;
}Uart;

static Uart uart;

void setBaud57600(void) {
#define BAUD 57600
#include <util/setbaud.h>
UBRR0H = UBRRH_VALUE;
UBRR0L = UBRRL_VALUE;

#ifdef USE_2X
UCSR0A |= (1<<U2X0);
#endif
#undef BAUD
}


void setBaud115200(void) {
#define BAUD 115200
#include <util/setbaud.h>
UBRR0H = UBRRH_VALUE;
UBRR0L = UBRRL_VALUE;

#ifdef USE_2X
UCSR0A |= (1<<U2X0);
#endif
#undef BAUD
}


/**
 * Initialize the Uart module
 **/
struct Uart* Uart_init(uint32_t baud) {
  cli();
  switch(baud) {
  case 57600:{
    setBaud57600();
    break;
  }
  case 115200:{
    setBaud115200();
    break;
  }
  default:
    return 0;
  }
  
  uart.rx_start=0;
  uart.rx_end=0;
  uart.rx_size=0;
  uart.tx_start=0;
  uart.tx_end=0;
  uart.tx_size=0;

  UCSR0A=0x00;
  UCSR0C=(1<<UCSZ01) | (1<<UCSZ00); // 8 bit data
  UCSR0B=(1<<RXEN0) | (1<<TXEN0) | (1<<RXCIE0);//enable rx and tx functions
  
  

  sei();
  return &uart;
}

/**
 * Insert c in the uart buffer, ready to be sent
 **/
void Uart_write(struct Uart* u, uint8_t c) {
  while(u->tx_size>=BUF_SIZE);
  u->tx_buffer[u->tx_end++]=c;
  u->tx_size++;
  UCSR0B |= (1<<UDRIE0); // Enable empty TX Interrupt
}

/**
 * Return the number of available bytes to read from the uart
 **/
uint8_t Uart_available(struct Uart* u) {
  return u->rx_size;
}

/**
 * reads the first character from the uart buffer
 **/
uint8_t Uart_read(struct Uart* u) {
  while(u->rx_size==0);
  uint8_t c=u->rx_buffer[u->rx_start];
  u->rx_start++;
  u->rx_size--;
  return c;
}

/**
 * Returns the number of free cells inside the tx buffer
 **/
uint16_t Uart_txFree(struct Uart* u) {
  return BUF_SIZE-u->tx_size;
}

ISR(USART0_RX_vect) { // Receive ISR
  uint8_t c=UDR0; // Received char
  if(uart.rx_size<BUF_SIZE) {
    uart.rx_buffer[uart.rx_end]=c;
    ++uart.rx_end;
    uart.rx_size++;
  }
}

ISR(USART0_UDRE_vect) { // Transmit ISR
  if(!uart.tx_size) { // No more bytes to send
    UCSR0B &= ~(1<<UDRIE0); // Disable Empty TX Interrupt
  } else {
    UDR0=uart.tx_buffer[uart.tx_start];
    ++uart.tx_start;
    --uart.tx_size;
  }  
}
