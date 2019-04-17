#include "serial.h"


int setupTermios(int fd, int speed, int parity) {
  struct termios tty;
  memset(&tty, 0, sizeof(tty));

  if(tcgetattr(fd, &tty) != 0) {
    printf("Error %d in tcgetattr.\n", errno);
    return -1;
  }

  switch (speed){
  case 57600:
    speed=B57600;
    break;
  case 115200:
    speed=B115200;
    break;
  default:
    printf("Baudrate not found: %d\n", speed);
    return -1;
  }

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


int setupSerial(char* device, int speed) {
  int fd = open(device, O_RDWR | O_NOCTTY | O_SYNC);   // Getting the file descriptor of the device

  if(fd < 0) {
    printf("Error %d opening %d.\n", errno, fd);
    return -1;
  }

  if(setupTermios(fd, speed, 0)) {   // Setting up the serial communication using termios
    printf("Error while setting up serial communication.\n");
    return -1;
  }

  sleep(2);
  tcflush(fd, TCIOFLUSH);
  
  return fd;
}
