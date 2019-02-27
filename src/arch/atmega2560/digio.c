/**
   digio.c
   ATMega2560 version
**/

#include "digio.h"
#include <avr/io.h>

/**
 * configure p as Input or Output
 **/
void digio_configurePin(uint8_t , PinMode) {

}
/**
 * Set the PORT Register of the pin:
 * is pin an OUTPUT ?
 *    then modify the output of the pin
 * is pin an INPUT ?
 *    then modify the pullup status of the pin
 **/
void digio_setPin(uint8_t , PinStatus) {

}
/**
 * Reads the PIN Register of the pin
 **/
uint8_t digio_readPin(uint8_t) {

}
