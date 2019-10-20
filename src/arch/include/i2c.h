/**
 * i2c.h
 * Two Wire Serial Interface 
 **/

#pragma once
#include <stdint.h>

typedef enum {
  MasterTx=0,
  MasterRx=1,
  SlaveTx=2,
  SlaveRx=3
}TWIMode;

typedef enum {
  StartSent=0x08,
  
}TWIStatus;

struct i2c;

struct i2c* i2c_init(TWIMode mode, uint8_t addr);

TWIStatus i2c_status(struct i2c*);

// Stores addr as the next slave address
void i2c_connect(uint8_t addr);

// Sends a stop signal in order to free the TWI bus
void i2c_disconnect(void);

// Returns the number of buffered received bytes
uint8_t i2c_available(void);

// Issue a SLA+R command, followed by
uint8_t i2c_read(uint8_t* dest, uint8_t num_bytes);

uint8_t i2c_write(uint8_t* src, uint8_t num_bytes);
