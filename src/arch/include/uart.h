/**
 * uart.h
 *
 * This section handle the low level mechanics of the UART module.
 * Different functions have to be made, in order to send and receive
 * bytes.
 **/

#pragma once
#include <stdint.h>

struct Uart;

/**
 * Initialize the Uart module
 **/
struct Uart* Uart_init(uint32_t);

/**
 * Insert c in the uart buffer, ready to be sent
 **/
void Uart_write(struct Uart*, uint8_t);

/**
 * Return the number of available bytes to read from the uart
 **/
uint8_t Uart_available(struct Uart*);

/**
 * reads the first character from the uart buffer
 **/
uint8_t Uart_read(struct Uart*);

/**
 * Returns the number of free cells inside the tx buffer
 **/
uint16_t Uart_txFree(struct Uart*);
