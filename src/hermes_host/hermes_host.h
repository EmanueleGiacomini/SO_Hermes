#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>

#include <unistd.h>
#include <errno.h>

#include "packet_handler.h"
#include "hermes_packets.h"

#define NUM_THREADS 1
#define NUM_DEVICES 2

// Defining indeces of devices
#define JOYSTICK 0
#define MEGA 1

#define SPEED 57600 // Baud rate

void sendPacket(int fd, PacketHandler* ph, PacketHeader* h);



