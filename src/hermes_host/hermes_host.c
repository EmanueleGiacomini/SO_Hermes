#include "hermes_host.h"
#include "joystick.h"

void* joystickRoutine(void *arg) {
	int fd = *(int*) arg;
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
			printf("An error occured while reading the event.\n");
			printf("Error code: %d\n", errno);
			exit(1);
		}
	}
	
	printf("-------------------------------\n");
	printf("Shutting down the program\n");
	printf("-------------------------------\n");
	printf("Bye bye ...\n");
	
	if(close(fd)) {
		printf("An error occured while closing the file descriptor.\n");
		printf("Error code: %d\n", errno);
		exit(1);
	}
	
	pthread_exit(NULL);
}


int main(int argc, char* argv[]) {
	pthread_t threads[NUM_THREADS]; // Add more threads incrementing NUM_THREADS, Thread#0 -> Joystick
	int ret, i;
	int fd = open("/dev/input/js1", O_RDONLY, O_NONBLOCK); 	// Getting the file descriptor of the joystick on js1
	
	if(fd < 0) {
		printf("Cannot find a joystick. Please connect one.\n");
		exit(1);
	}
	
	for (i=0; i<NUM_THREADS; ++i) {
		if(!i) ret = pthread_create(&threads[i], NULL, joystickRoutine, (void*)&fd); // Creating Thread#0 to manage a joystick
		if (ret != 0) {
			printf("ERROR: code %d\n", ret); 
			exit(1);
		}
	}
	
	printf("I'm the host, doing some stuff\n");
	int x = 0;
	while(x < 100000000) {
		++x;
	}
	printf("Done!\n");
	
	
	pthread_exit(NULL);
	return 0;
}
