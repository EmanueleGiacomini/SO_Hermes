/**
 * encoder.c
 **/

#include "encoder.h"
#include <avr/io.h>
#include <avr/interrupt.h>

#define NUM_ENCODERS 4


/**
 * Encoder mask for Register B (53-52-51-50-10-11-12-13)
 * We use the following configuration:
 *
 * /-J3-/  /-J2-/  /-J1-/  /-J0-/ 
 * 1 - 1 - 0 - 0 - 0 - 0 - 0 - 0    <- Encoder mask
 * 13  12  11  10  50  51  52  53   <- Arduino mapped pins
 * PB7 PB6 PB5 PB4 PB3 PB2 PB1 PB0  <- ATmega2560 PORTB
 * regvalue: 0xC0
 **/
#define ENCODER_MASK 0xC0
uint8_t _encoder_prev;// Previous state of PINB
// Current encoder value
int16_t _encoder_current_value[NUM_ENCODERS];
// Sampled value
int16_t _encoder_sampled_value[NUM_ENCODERS];

/**
 * Initializes the encoders by activating PCI (Pin Change Interrupt) on encoder
 * pins.
 **/
void Encoder_init(void) {
  cli();
  for(int i=0;i<NUM_ENCODERS;++i) {
    _encoder_current_value[i]=0;
    _encoder_sampled_value[i]=0;
  }
  DDRB &= ~ENCODER_MASK;// set encoder pins as INPUT
  PORTB |= ENCODER_MASK;// enable pull-up resistors on encoder pins
  PCICR |= (1<<PCIE0);// set interrupt on change for PCMSK0
  PCMSK0 |= ENCODER_MASK;// set PCINT0 to trigger an interrupt on state change
  sei();
  return;
}

/**
 * sample the encoders
 **/
void Encoder_sample(void) {
  cli();
  for(int i=0;i<NUM_ENCODERS;++i) {
    _encoder_sampled_value[i]=_encoder_current_value[i];
  }
  sei();
}

/**
 * Get sampled values from encoders
 **/
int16_t Encoder_getValue(uint8_t idx) {
  return _encoder_sampled_value[idx];
}

/**
 * Get the number of encoders attached to the MCU
 **/
uint8_t Encoder_numEncoders(void) {
  return NUM_ENCODERS;
}

static const int8_t _transition_table []= {  
  0,  //0000
  1,  //0001
  -1, //0010
  0,  //0011
  -1, //0100
  0,  //0101
  0,  //0110
  1,  //0111
  1,  //1000
  0,  //1001
  0,  //1010
  -1, //1011
  0,  //1100
  -1, //1101
  1,  //1110
  0   //1111
};

ISR(PCINT0_vect) {
  cli();
  uint8_t port_value=PINB&ENCODER_MASK;
  register uint8_t c_value=port_value;
  register uint8_t prev_value=_encoder_prev;
  register const int8_t* ttable=_transition_table;
  register int16_t* curr_enc=_encoder_current_value;
  for(uint8_t i=0;i<NUM_ENCODERS;++i) {
    // build the following data:
    // first 2 bits of prev_value | first 2 bits of c_value
    uint8_t table_idx=(c_value&0x03)|(prev_value&0x03)<<2;
    *curr_enc+=ttable[table_idx];
    ++curr_enc;
    c_value>>=2;
    prev_value>>=2;
    
  }
  _encoder_prev=port_value;
  sei();
}


