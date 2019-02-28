#include "joystick.h"


int main(int argc, char* argv[]) {
	int fd = open("/dev/input/js1", O_RDONLY, O_NONBLOCK); 	// Getting the file descriptor of the controller on js1
	
	if(fd < 0) {
		printf("Cannot find a joystick. Please connect one.\n");
		exit(1);
	}
	
	int endit = 0;
	struct js_event e;
	
	printf("-------------------------------\n");
	printf("Welcome to the joystick program\n");
	printf("-------------------------------\n");
	
	while(1) {
		while(read(fd, &e, sizeof(e)) > 0) {
			
			if(e.type == JS_EVENT_BUTTON && e.value == BTN_PRES) { 	// Verifying that a button has been pressed
				if(e.number == BTN_PS) {
					endit = 1;
					break;
				}
				
				printButton(e.number);
				
			}
			
		}	
		
		if(endit) break;
		
		if(errno != EAGAIN) {
			printf("An error occured\n");
			printf("Error code: %d\n", errno);
			exit(1);
		}

	}
	
	printf("-------------------------------\n");
	printf("Shutting down the program\n");
	printf("-------------------------------\n");
	printf("Bye bye ...\n");
	
	return 0;
}
