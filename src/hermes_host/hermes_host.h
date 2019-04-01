#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>

#include <errno.h>

#define NUM_THREADS 1
#define NUM_DEVICES 2

// Defining indeces of devices
#define JOYSTICK 0
#define MEGA 1

#define SPEED 57600 // Baud rate


int sendPacket(int fd, char* buf, int len); // Just a test





