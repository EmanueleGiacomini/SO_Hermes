/**
 * encoder.h
 **/

#pragma once
#include <stdint.h>

/**
 * Initializes the encoders by activating PCI (Pin Change Interrupt) on encoder
 * pins.
 **/
void Encoder_init(void);

/**
 * sample the encoders
 **/
void Encoder_sample(void);

/**
 * Get sampled values from encoders
 **/
int16_t Encoder_getValue(uint8_t);

/**
 * Get the number of encoders attached to the MCU
 **/
uint8_t Encoder_numEncoders(void);
