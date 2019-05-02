/**
 * timer.h
 *
 * This section handle the timer modules of the uC.
 * Every instance of timer let the user execute
 * an ISR (Interrupt Service Routine) every tot ms
 * Of course ISR routines should be fast in order not to
 * block the main execution
 **/

#pragma once
#include <stdint.h>

struct Timer;

typedef void (*TimerFn)(void*);

void Timer_init(void);

struct Timer* Timer_create(uint16_t duration_ms,
                           TimerFn timer_fn,
                           void* timer_args);

void Timer_destroy(struct Timer*);

void Timer_start(struct Timer*);

void Timer_stop(struct Timer*);
