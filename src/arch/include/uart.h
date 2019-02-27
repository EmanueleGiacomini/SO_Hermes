/**
 * uart.h
 *
 * This section handle the low level mechanics of the UART module.
 * Different functions have to be made, in order to send and receive
 * bytes.
 **/

#pragma once
#include <stdint.h>

/**
 * Insert c in the uart buffer, ready to be sent
 **/
void uart_write(uint8_t c);

/**
 * Return the number of available bytes to read from the uart
 **/
uint8_t uart_available();

/**
 * reads the first character from the uart buffer
 **/
uint8_t uart_read();
