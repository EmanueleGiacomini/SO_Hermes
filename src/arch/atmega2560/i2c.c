/**
 * i2c.c
 **/

#include "i2c.h"
#include <avr/io.h>

#define BUF_SIZE 256

#define TWI_START_SENT 0x08
#define TWI_REP_START_SENT 0x10
// Master Transmitter
#define TWI_MAT_SLAW_ACK 0x18
#define TWI_MAT_SLAW_NACK 0x20
#define TWI_MAT_DATA_ACK 0x28
#define TWI_MAT_DATA_NACK 0x30
#define TWI_MAT_ARBI_LOST 0x38
// Master Receiver
#define TWI_MAR_SLAR_ACK 0x40
#define TWI_MAR_SLAR_NACK 0x48
#define TWI_MAR_DATA_ACK 0x50
#define TWI_MAR_DATA_NACK 0x58
#define TWI_MAR_ARBI_LOST 0x38

typedef struct i2c{
  uint8_t* twbr; // TW Bit Rate Register
  uint8_t* twcr; // TW Control Register
  uint8_t* twsr; // TW Status Register
  uint8_t* twdr; // TW Data Register
  
  TWIMode mode;
  TWIStatus status;
  uint8_t addr;
  uint8_t targ_addr;

  uint8_t tx_buf[BUF_SIZE];
  uint8_t tx_start;
  uint8_t tx_end;
  uint8_t tx_size;

  uint8_t rx_buf[BUF_SIZE];
  uint8_t rx_start;
  uint8_t rx_end;
  uint8_t rx_size;
}TWIHandler;

static TWIHandler twi_0 = {
  .twbr=&TWBR,
  .twcr=&TWCR,
  .twsr=&TWSR,
  .twdr=&TWDR,
  .mode=0,
  .addr=0,
  .targ_addr=0
};

void TWIHandler_init(TWIHandler* t, TWIMode mode, uint8_t addr) {
  // Set internal TWI interface register
  *t->twbr=0x48; // bitrate = 72 = 0x48 for 100kHz channel speed
  *t->twcr=0;
  *t->twcr=(1<<TWEN); // Enable TW Interface
  
  // Set TWIHandler interface variables
  t->mode = mode;
  t->addr = addr;
  t->targ_addr = 0x00;// standard broadcast channel

  t->tx_start=0;
  t->tx_end=0;
  t->tx_size=0;
  t->rx_start=0;
  t->rx_end=0;
  t->rx_size=0;
}

void TWIHandler_setTarget(TWIHandler* t, uint8_t targ_addr) {
  t->targ_addr=targ_addr;
  return;
}

uint8_t TWIHandler_txFree(TWIHandler* t) {
  return BUF_SIZE - t->tx_size;
}

uint8_t TWIHandler_rxFree(TWIHandler* t) {
  return BUF_SIZE - t->rx_size;
}

uint8_t TWIHandler_txAvailable(TWIHandler* t) {
  return t->tx_size;
}

uint8_t TWIHandler_available(TWIHandler* t) {
  return t->rx_size;
}

void TWIHandler_txByte(TWIHandler* t) {
  *t->twdr=t->tx_buf[t->tx_start++];
  ++t->tx_size;
  return;
}

void TWIHandler_addRxByte(TWIHandler* t, uint8_t dat) {
  t->rx_buf[t->rx_end++]=dat;
  return;
}

// TWIHandler hardware commands

void TWIHandler_start(TWIHandler* t) {
  *t->twcr |=
    (1 << TWEN) | // Enable TWI interface
    (1 << TWIE) | // Enable TWI interrupts
    (1 << TWINT) | // Clear interrupt flag
    (1 << TWSTA); // Send start signal
  
}

void TWIHandler_stop(TWIHandler* t) {
  *t->twcr |= (1 << TWSTO);
  *t->twcr &= ~(1 << TWIE) & ~(1 << TWINT);
}




struct i2c* i2c_init(TWIMode mode, uint8_t addr) {
  TWIHandler_init(&twi_0, mode, addr);
  return &twi_0;
}

// Stores addr as the next slave address
void i2c_connect(uint8_t addr) {
  TWIHandler_setTarget(&twi_0, addr);
  return;
}

// Sends a stop signal in order to free the TWI bus
void i2c_disconnect(void) {
  return;
}

// Returns the number of buffered received bytes
uint8_t i2c_available(void) {
  return TWIHandler_available(&twi_0);
}

uint8_t i2c_read(uint8_t* dest, uint8_t num_bytes) {
  uint8_t fill_slots = TWIHandler_rxAvailable(&twi_0);
  if(num_bytes > fill_slots) {
    num_bytes = fill_slots;
  }
  memcpy(dest, t->rx_buf, num_bytes);
  
  return;
}

uint8_t i2c_write(uint8_t* src, uint8_t num_bytes) {
  uint8_t empty_slots = TWIHandler_txFree(&twi_0);
  if(num_bytes > empty_slots) {
    num_bytes = empty_slots;
  }
  memcpy(twi_0.tx_buf, src, num_bytes);

  TWIHandler_start(&twi_0); // Send start signal and wake the FSM
  return num_bytes;
}

static void TWIHandler_resetPointer(TWIHandler* t) {
  t->rx_start=0;
  return;
}

ISR(TWI_vect) {
  switch (TWSR & 0xF8) {
  case TWI_START_SENT:

  case TWI_REP_START_SENT:
    TWIHandler_resetPointer(&twi_0);

  case TWI_MAT_SLAW_ACK:
    if(TWIHandler_txAvailable(&twi_0)) {
      TWIHandler_txByte(&twi_0); // insert data
    }
  }
}

