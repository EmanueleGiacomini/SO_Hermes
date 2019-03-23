/**
 * spi.h
 *
 * This section handle the low level mechanics of SPI module.
 * TODO: Add explanations of SPI module
 **/

#pragma once
#include <stdint.h>

#define BUF_SIZE 256


enum SPI_MODE {
  MASTER,
  SLAVE
};

struct Spi;

/**
 * Initialize the Spi module
 **/
struct Spi* Spi_init(uint32_t);

/**
 * Insert c in the spi buffer, ready to be sent
 **/
void Spi_write(struct Spi*, uint8_t);

/**
 * Return the number of available bytes to read from the spi
 **/
uint8_t Spi_available(struct Spi*);

/**
 * reads the first character from the spi buffer
 **/
uint8_t Spi_read(struct Spi*);

/**
 * Returns the number of free cells inside the tx buffer
 **/
uint16_t Spi_txFree(struct Spi*);





//------------------------ Low level functions ------------------------// 

/**
 * Insert c in the spi buffer, ready to be sent
 **/
void spi_write(uint8_t c);

/**
 * Return the number of available bytes to read from the spi bus
 **/
uint8_t spi_available();

/**
 * reads the first character from the spi buffer
 **/
uint8_t spi_read();

/**
 * Initialize SPI master
 **/

void spi_init_master();

/**
 * Initialize SPI slave
 **/

void spi_init_slave();
