#include "joystick.h"


MotorControlPacket* alterPacket(js_event * event) {

  MotorControlPacket *mcp = malloc(sizeof(MotorControlPacket)); // TODO: Call a function to create a new MotorControlPacket
  mcp->mode = 1;

  int16_t val = MAPPED_VALUE(event->value);

  if(event->type == JS_EVENT_BUTTON) {
    switch(event->number) {
    case BTN_X:
      mcp->speed = 200;
      break;
    default:
      break;
    }

  } else if(event->type == JS_EVENT_AXIS) {
    switch(event->number) {
    case AXIS_LX:
      break;
    case AXIS_LY:
      break;
    case AXIS_L2:
      mcp->speed = val;
      break;
    case AXIS_RX:
      break;
    case AXIS_RY:
      break;
    case AXIS_R2:
      mcp->speed = val;
      break;
    default:
      break;
    }


  } else {
    printf("Event type not avaiable. \n");
    return NULL;
  }

  return mcp;
}


void printButton(uint8_t btn) {
  switch(btn) {
  case BTN_X:
    printf("X\n");
    break;
  case BTN_O:
    printf("CIRCLE\n");
    break;
  case BTN_TR:
    printf("TRIANGLE\n");
    break;
  case BTN_SQ:
    printf("SQUARE\n");
    break;
  case BTN_L1:
    printf("L1\n");
    break;
  case BTN_R1:
    printf("R1\n");
    break;
  case BTN_L2:
    printf("L2\n");
    break;
  case BTN_R2:
    printf("R2\n");
    break;
  case BTN_SEL:
    printf("SELECT\n");
    break;
  case BTN_STRT:
    printf("START\n");
    break;
  case BTN_L3:
    printf("L3\n");
    break;
  case BTN_R3:
    printf("R3\n");
    break;
  case UP:
    printf("UP\n");
    break;
  case DOWN:
    printf("DOWN\n");
    break;
  case LEFT:
    printf("LEFT\n");
    break;
  case RIGHT:
    printf("RIGHT\n");
    break;
  default:
    printf("Button not found.\n");
    break;
  }
}

