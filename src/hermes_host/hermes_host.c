#include "hermes_host.h"
#include "joystick.h"


int setupSerial(int fd, int speed, int parity) {
	struct termios tty;
	memset(&tty, 0, sizeof(tty));
	
	if(tcgetattr(fd, &tty) != 0) {
		printf("Error %d in tcgetattr.\n", errno);
		return -1;
	}
	
	speed = B57600;
	
	cfsetospeed(&tty, speed);
	cfsetispeed(&tty, speed);
	cfmakeraw(&tty);
	
	tty.c_cflag &= ~(PARENB | PARODD);
	tty.c_cflag |= parity;
	tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;
	
	if(tcsetattr(fd, TCSANOW, &tty) != 0) {
		printf("Error %d in tcsetattr.\n", errno);
		return -1;
	}
	
	return 0;
}


int sendPacket(int fd, char* buf, int len) {
	printf("Len is %d \n", len);
	uint8_t* test = (uint8_t*) buf;
	int i;
	for(i = 0; i<len; i++){	
		write(fd, &test[i], 1);
	}
	ssize_t res ;
	if(res == -1) {
		printf("Error %d writing on %d.\n", errno, fd);
		printf("%s \n", strerror(errno));
		return -1;
	}
	
	printf("PKT SENT\n");
	return 0;
}


void* mainRoutine(void *arg) {
	int* fds = (int*)arg;
	int endit = 0, i = 0;
	js_event e;
	//printf("Desc joy = %d, Desc Ard = %d \n", fds[JOYSTICK], fds[MEGA]);
	
	ssize_t res;
	int len = 17;
	char buf[17]="Ciao sono un pkt\n";
	
	
	printf("-------------------------------\n");
	printf("Welcome to HERMES!\n");
	printf("-------------------------------\n");

	//Check filed descriptor status
	//printf("1)fcntl of %d is : %d \n", fds[MEGA], fcntl(fds[MEGA], F_GETFD));
	
	while(1) {
		while(read(fds[JOYSTICK], &e, sizeof(e)) > 0) {
			if(e.type == JS_EVENT_BUTTON && e.value == BTN_PRES) { 	// Verifying that a button has been pressed
				if(e.number == BTN_PS) {
					endit = 1;
					break;
				}
				sendPacket(fds[MEGA], buf, len); 
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
	
	
	for(i=0; i<NUM_DEVICES; ++i) {  // Closing all devices
		if(close(fds[i])) {
			printf("An error occured while closing the file descriptor %d.\n", fds[i]);
			printf("Error code: %d\n", errno);
			exit(1);
		}
	}
	
	printf("-------------------------------\n");
	printf("Shutting down HERMES!\n");
	printf("-------------------------------\n");
	printf("Bye bye ...\n"); 
	
	pthread_exit(NULL);
}


int main(int argc, char* argv[]) {
	pthread_t threads[NUM_THREADS]; // Add more threads incrementing NUM_THREADS, Thread#0 -> Joystick
	int ret, i=0, fds[NUM_DEVICES];
	
	int joy_fd = open("/dev/input/js1", O_RDONLY | O_NONBLOCK);  // Getting the file descriptor of the joystick on js1
	if(joy_fd < 0) {
		printf("Error %d opening %d.\n", errno, joy_fd);
		printf("Cannot find a joystick. Please connect one.\n");
		exit(1);
	} else fds[i++] = joy_fd;
	
	int mega_fd = open("/dev/ttyACM0" , O_RDWR | O_NOCTTY | O_SYNC);  // Getting the file descriptor of the Arduino on js0
																  // O_RDWR | O_NONBLOCK
	if(mega_fd < 0) {
		printf("Error %d opening %d.\n", errno, mega_fd);
		printf("Cannot find an Arduino. Please connect one.\n");
		exit(1);
	} else fds[i++] = mega_fd;
	
	if(setupSerial(mega_fd, SPEED, 0)) {  // Setting up the serial communication
		printf("Error while setting up serial communication.\n");
		exit(1);
	}
	
	
	
	for (i=0; i<NUM_THREADS; ++i) {
		if(!i) ret = pthread_create(&threads[i], NULL, mainRoutine, (void*)fds); // Creating Thread#0 to manage a joystick
		if (ret != 0) {
			printf("ERROR: code %d\n", ret); 
			exit(1);
		}
	}
	
	/*
	printf("I'm the host, doing some stuff\n");
	int x = 0;
	while(x < 100000000) {
		++x;
	}
	printf("Done!\n");
	*/
	
	pthread_exit(NULL);
	return 0;
}
