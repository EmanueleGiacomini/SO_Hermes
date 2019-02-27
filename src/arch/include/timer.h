/**
 * timer.h
 *
 * This section handle the timer modules of the uC.
 * Every instance of timer let the user execute
 * an ISR (Interrupt Service Routine) every tot ms
 * On ATMega2560 it is possible to use the devices:
 * 1) timer_4
 * 2) timer_5
 * Of course ISR routines should be fast in order not to
 * block the main execution
 **/

#pragma once
#include <stdint.h>
