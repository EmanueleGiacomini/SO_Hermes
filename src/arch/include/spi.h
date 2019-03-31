#include <avr/io.h>

// SPI Pins
#define SPI_DDR DDRB
#define SPI_PORT PORTB
#define SPI_MISO PB3 //PB3 - PIN 50
#define SPI_MOSI PB2 //PB2 - PIN 51
#define SPI_SCK PB1 //PB1 - PIN 52
#define SPI_SS PB0 //PB0 - PIN 53

extern void spi_init(void);
extern uint8_t spi_writereadbyte(uint8_t data);
