#include "joystick.h"


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
	}	
}

