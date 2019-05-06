/**
 * timer.c
 **/

#include "timer.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>

#define NUM_TIMERS 8

static uint8_t mask_read(uint8_t* mask, uint8_t index) {
  return (*mask & (0x1<<index))!=0;
}

static void mask_set(uint8_t* mask, uint8_t index) {
  *mask |= (0x1<<index);
  return;
}

static void mask_clear(uint8_t* mask, uint8_t index) {
  *mask &= ~(0x1<<index);
  return;
}

typedef struct Timer{
  uint8_t timer_num;
  uint8_t stop_flag;
  uint16_t duration_ms;
  TimerFn fn;
  void* args;
}Timer;

uint8_t timers_mask=0;
Timer timers[NUM_TIMERS];
volatile uint8_t timers_counter=0;

/**
 * Uses TIMER5 to launch an interrupt every 1mS
 **/
void Timer_init() {
  cli();
  TCCR5A=0;
  TCCR5B=0;

  // CTC-Mode | Prescaler=8 => clock_period=0.5uS
  TCCR5B=(1<<WGM12) | (1<<CS51);
  // To achieve 1ms period we need OCR5A to be 1999 (1000 ms / 0.5 us - 1 = 1999 )
  OCR5A=1999;
  // Enable Timer Output Compare A interrupt mask on TIMSK5
  TIMSK5|=(1<<OCIE5A);
  sei();
}

struct Timer* Timer_create(uint16_t duration_ms,
                           TimerFn timer_fn,
                           void* timer_args) {
  cli();
  for(int i=0;i<NUM_TIMERS;++i) {
    if(mask_read(&timers_mask, i)==0) {// timers_mask[i] == 0 => timer not used
      mask_set(&timers_mask, i);
      memset(&timers[i], 0, sizeof(Timer));
      timers[i].timer_num=i;
      timers[i].stop_flag=1;// function is initially in Steady state
      timers[i].duration_ms=duration_ms;
      timers[i].fn=timer_fn;
      timers[i].args=timer_args;
      sei();
      return &timers[i];
    }
  }
  sei();
  return 0; // No free timers slot
}

void Timer_destroy(struct Timer* t) {
  cli();
  mask_clear(&timers_mask, t->timer_num);
  sei();
}

void Timer_start(struct Timer* t) {
  cli();
  t->stop_flag=0;
  sei();
}

void Timer_stop(struct Timer* t) {
  cli();
  t->stop_flag=1;
  sei();
}

volatile uint16_t elapsed_time=0;


ISR(TIMER5_COMPA_vect) {
  elapsed_time++;
  for(int i=0;i<NUM_TIMERS;++i) {
    if(mask_read(&timers_mask, i)==1) {
      // check if current struct can be executed
      if(timers[i].stop_flag==0 && (elapsed_time % timers[i].duration_ms)==0) {
        (*timers[i].fn)(timers[i].args);
      }      
    }
  }
}

