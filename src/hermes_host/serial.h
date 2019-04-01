#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>
#include <errno.h>


int setupTermios(int fd, int speed, int parity);
int setupSerial(char* device, int speed);
