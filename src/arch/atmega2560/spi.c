
#include "spi.h"
#include <avr/io.h>
#include <avr/interrupt.h>


typedef struct Spi{
  uint8_t rx_buffer[BUF_SIZE];
  uint8_t rx_start;
  uint8_t rx_end;
  uint8_t rx_size;

  uint8_t tx_buffer[BUF_SIZE];
  uint8_t tx_start;
  uint8_t tx_end;
  uint8_t tx_size;
}Spi;

static Spi spi;


void spi_write(uint8_t c) {
  
  SPDR = c; // Loading data in the buffer
  
  while( !(SPSR & (1<<SPIF)) ); // Waiting till the transmission is complete
  
}

uint8_t spi_read() {

  return SPDR; // Returns the value in the buffer
  
}

void spi_init_master() {
  
  DDRB = (1<<5) | (1<<3); // MOSI, SCK as output
  
  SPCR = (1<<SPE) | (1<<MSTR) | (1<<SPR0) | (1<<SPIE); // SPI on, Master on, Interrupts on
  
}

void spi_init_slave() {
  DDRB = (1<<6); // MISO as output
  
  SPCR = (1 <<SPE); // SPI on
}



struct Spi* Spi_init(uint32_t mode) {
  cli();
  
  switch(mode) {
  case MASTER:{
    spi_init_master();
    break;
  }
  case SLAVE:{
    spi_init_slave();
    break;
  }
  default:
    return 0;
  }
  
  spi.rx_start=0;
  spi.rx_end=0;
  spi.rx_size=0;
  spi.tx_start=0;
  spi.tx_end=0;
  spi.tx_size=0;  

  sei();
  return &spi;
}

/**
 * Insert c in the spi buffer, ready to be sent
 **/
void Spi_write(struct Spi* s, uint8_t c) {
  while(s->tx_size>=BUF_SIZE);
  s->tx_buffer[s->tx_end++]=c;
  s->tx_size++;
}

/**
 * Return the number of available bytes to read from the spi
 **/
uint8_t Spi_available(struct Spi* s) {
  return s->rx_size;
}

/**
 * reads the first character from the spi buffer
 **/
uint8_t Spi_read(struct Spi* s) {
  while(s->rx_size==0);
  uint8_t c=s->rx_buffer[s->rx_start];
  s->rx_start++;
  s->rx_size--;
  return c;
}

/**
 * Returns the number of free cells inside the tx buffer
 **/
uint16_t Uart_txFree(struct Spi* s) {
  return BUF_SIZE-s->tx_size;
}



//  Interrupt Service Routine for the SPI

ISR(SPI_STC_vect) {
  if(spi.tx_size) {
    spi_write(spi.tx_start);
    ++spi.tx_start;
    --spi.tx_size;
  }
  
  uint8_t read_value = spi_read();
  if(spi.rx_size < BUF_SIZE) {
    spi.rx_buffer[spi.rx_end] = read_value;
    ++spi.rx_end;
    spi.rx_size++;
  }
}
