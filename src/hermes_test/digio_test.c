/**
 * digio_test.c
 **/

#include "digio.h"

void main() {
  digio_configurePin(13, Output);
  digio_setPin(13, High);
}
