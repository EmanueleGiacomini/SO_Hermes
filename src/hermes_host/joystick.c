#include "joystick.h"


int alterPacket(js_event* event, MotorControlPacket* mcp) {
  int16_t val = MAPPED_VALUE(event->value);

  if(event->type == JS_EVENT_BUTTON) {
    switch(event->number) {
    case BTN_STRT:
      mcp->speed = 0;
      break;
    case BTN_X:
      mcp->speed = 50;
      break;
    case BTN_O:
      mcp->speed = 100;
      break;
    case BTN_TR:
      mcp->speed = 150;
      break;
    case BTN_SQ:
      mcp->speed = 200;
      break;
    case UP:
      mcp->speed += 10;
      break;
    case DOWN:
      mcp->speed -= 10;
      break;
    case LEFT:
      mcp->mode = 0;
      break;
    case RIGHT:
      mcp->mode = 1;
      break;
    default:
      return 1;
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
      return 1;
    }

  } else {
    printf("Event type not avaiable. \n");
    return 1;
  }

  return 0;
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

