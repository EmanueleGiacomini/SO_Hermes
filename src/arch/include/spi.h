/**
 * spi.h
 *
 * This section handle the low level mechanics of SPI module.
 * TODO: Add explanations of SPI module
 **/


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
