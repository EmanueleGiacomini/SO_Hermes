#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <hermes_packets.h>

#define JS_EVENT_BUTTON         0x01    // Button pressed/released 
#define JS_EVENT_AXIS           0x02    // Joystick moved 
#define JS_EVENT_INIT           0x80    // Initial state of the device 

#define BTN_PRES  	0x01  	// Value when the button is held down
#define BTN_REL	  	0x00  	// Value when the button is released

#define MAX_AXIS_VALUE 32767  // Max axis value
#define MAPPED_VALUE(x) ((x > 0)? (((x*127)/MAX_AXIS_VALUE) + 128) : (((x*127)/MAX_AXIS_VALUE) + 127) ) // Mapping value to 0-255

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

enum CNTRL_AXES {   // List of axes avaiable
  AXIS_LX,
  AXIS_LY,
  AXIS_L2,
  AXIS_RX,
  AXIS_RY,
  AXIS_R2
};

typedef struct {
  uint32_t time;     // Event timestamp in milliseconds 
  int16_t value;    // Value 
  uint8_t type;      // Event type
  uint8_t number;    // Axis/button number -> CNTRL_BTNS/AXES 
} js_event;


void printButton(uint8_t btn);  // Function that prints the value of the button
int alterPacket(js_event* event, MotorControlPacket* mcp); // Function that transform an event to a desired MotorControlPacket
                              



