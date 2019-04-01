#include "spi.h"
#include <avr/io.h>
#include <avr/interrupt.h>


void spi_init() {
    SPI_DDR &= ~((1<<SPI_MOSI) | (1<<SPI_MISO) | (1<<SPI_SS) | (1<<SPI_SCK)); //input
    SPI_DDR |= ((1<<SPI_MOSI) | (1<<SPI_SS) | (1<<SPI_SCK)); //output

    SPCR = ((1<<SPE)|               // SPI Enable
            (0<<SPIE)|              // SPI Interupt Enable
            (0<<DORD)|              // Data Order (0:MSB first / 1:LSB first)
            (1<<MSTR)|              // Master/Slave select
            (0<<SPR1)|(1<<SPR0)|    // SPI Clock Rate
            (0<<CPOL)|              // Clock Polarity (0:SCK low / 1:SCK hi when idle)
            (0<<CPHA));             // Clock Phase (0:leading / 1:trailing edge sampling)

    SPSR = (1<<SPI2X); // Double SPI Speed Bit
}

uint8_t spi_writereadbyte(uint8_t data) {
    SPDR = data; // Loading data in the buffer
    while((SPSR & (1<<SPIF)) == 0); // Waiting till the transmission is complete
    return SPDR; // Read data
}
