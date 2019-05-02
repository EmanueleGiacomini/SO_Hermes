#include "hermes_host.h"
#include "serial.h"
#include "joystick.h"


void sendPacket(int fd, PacketHandler* ph, PacketHeader* h) {
  PacketHandler_sendPacket(ph, h);
  uint8_t bytes = PacketHandler_txSize(ph); 
  for(int i=0; i<bytes; i++) {
    uint8_t c = PacketHandler_writeByte(ph);
    write(fd, &c, 1);
  }
}

void* mainRoutine(void *arg) {
  int* fds = (int*)arg;
  int endit = 0, i = 0;
  js_event e;

  printf("-------------------------------\n");
  printf("Welcome to HERMES!\n");
  printf("-------------------------------\n");

  MotorControlPacket mcp;
  
  PacketHeader ph = {
    .seq = 0,
    .id = ID_MOTOR_CONTROL_PACKET,
    .size = sizeof(mcp),
    .dest_addr = 0,
    .checksum = 0
  };
  
  mcp.h = ph;
  PacketHandler_init(&ph);
   
  while(1) {
    while(read(fds[JOYSTICK], &e, sizeof(e)) > 0) {
      //MotorControlPacket mcp;
      if(e.type == JS_EVENT_BUTTON && e.value == BTN_PRES) { 	// Verifying that a button has been pressed
        if(e.number == BTN_PS) {
          endit = 1;
          break;
        }
        
        if(!alterPacket(&e, &mcp)) sendPacket(fds[MEGA], &ph, &mcp.h);
        
        /* test print
        printf("time -> %d \n", e.time);
        printf("value -> %d \n", e.value);
        printf("type -> %d \n", e.type);
        printf("number -> %d \n", e.number);
        */
        
      }
      
      if(e.type == JS_EVENT_AXIS) {   // Verifying that an axis has been moved
        /*
        if(e.number == AXIS_LX || e.number == AXIS_RX) {
            if(val > 128) printf("RIGHT\n");
            if(val < 128) printf("LEFT\n");
        }
        if(e.number == AXIS_LY || e.number == AXIS_RY) {
            if(val > 128) printf("DOWN\n");
            if(val < 128) printf("UP\n");
        }
        
        if(!alterPacket(&e, &mcp)) sendPacket(fds[MEGA], &ph, &mcp.h);
        */
      }
      
      printf("\33[2K\rSpeed is %d",mcp.speed);
      fflush(stdout);
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

  printf("\n");
  printf("-------------------------------\n");
  printf("Shutting down HERMES!\n");
  printf("-------------------------------\n");
  printf("Bye bye ...\n");

  pthread_exit(NULL);
}


int main(int argc, char* argv[]) {
  pthread_t threads[NUM_THREADS]; // Add more threads incrementing NUM_THREADS, Thread#0 -> Joystick
  int ret, i, fds[NUM_DEVICES];
  
  int joy_fd = open("/dev/input/js1", O_RDONLY | O_NONBLOCK);  // Getting the file descriptor of the joystick on js1
  if(joy_fd < 0) {
    printf("Error %d opening %d.\n", errno, joy_fd);
    printf("Cannot find a joystick. Please connect one.\n");
    exit(1);
  } else fds[JOYSTICK] = joy_fd;
  
  int mega_fd = setupSerial("/dev/ttyACM0", SPEED);
  if (mega_fd >= 0) fds[MEGA] = mega_fd;
  
  // If necessary insert here new file descriptors
  
  for (i=0; i<NUM_THREADS; ++i) {
    if(!i) ret = pthread_create(&threads[i], NULL, mainRoutine, (void*)fds); // Creating Thread#0 to manage a joystick
    if (ret != 0) {
      printf("ERROR: code %d\n", ret);
      exit(1);
    }
  }
  
  pthread_exit(NULL);
  return 0;
}
