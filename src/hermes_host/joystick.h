#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>

#define JS_EVENT_BUTTON         0x01    /* button pressed/released */
#define JS_EVENT_AXIS           0x02    /* joystick moved */
#define JS_EVENT_INIT           0x80    /* initial state of device */

#define BTN_PRES  	0x01  	// Value when the button is held down
#define BTN_REL	  	0x00  	// Value when the button is released

enum CNTRL_BTNS {  // List of buttons avaiable
	BTN_X,
	BTN_O,
	BTN_TR,
	BTN_SQ,
	BTN_L1,
	BTN_R1,
	BTN_L2,
	BTN_R2,
	BTN_SEL,
	BTN_STRT,
	BTN_PS,
	BTN_L3,
	BTN_R3,
	UP,
	DOWN,
	LEFT,
	RIGHT
};


struct js_event {
		uint32_t time;     /* event timestamp in milliseconds */
		int16_t value;    /* value */
		uint8_t type;      /* event type */
		uint8_t number;    /* axis/button number */
};


void printButton(uint8_t btn);  // Function that prints the number of the button




