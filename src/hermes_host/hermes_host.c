#include "hermes_host.h"
#include "serial.h"
#include "joystick.h"
#include "packet_handler.h"



int sendPacket(int fd, char* buf, int len) {
  //printf("Len is %d \n", len);
  uint8_t* test = (uint8_t*) buf;
  int i;
  for(i = 0; i<len; i++) {
    write(fd, &test[i], 1);
  }

  printf("PKT SENT\n");
  return 0;
}


void* mainRoutine(void *arg) {
  int* fds = (int*)arg;
  int endit = 0, i = 0;
  js_event e;
  //printf("Desc joy = %d, Desc Ard = %d \n", fds[JOYSTICK], fds[MEGA]);
  
  
  //int len = 17;
  //char buf[17]="Ciao sono un pkt\n";


  printf("-------------------------------\n");
  printf("Welcome to HERMES!\n");
  printf("-------------------------------\n");

  //Check filed descriptor status
  //printf("1)fcntl of %d is : %d \n", fds[MEGA], fcntl(fds[MEGA], F_GETFD));
  PacketHandler ph;
  PacketHandler_init(&ph);
   
  while(1) {
    while(read(fds[JOYSTICK], &e, sizeof(e)) > 0) {
      MotorControlPacket mcp;
      if(e.type == JS_EVENT_BUTTON && e.value == BTN_PRES) { 	// Verifying that a button has been pressed
        if(e.number == BTN_PS) {
          endit = 1;
          break;
        }
        
        /* test print
        printf("time -> %d \n", e.time);
        printf("value -> %d \n", e.value);
        printf("type -> %d \n", e.type);
        printf("number -> %d \n", e.number);
        */
        
        
        if(e.number == BTN_X) {
          mcp = alterPacket(&e);
          
          printf("Speed is %d \n",mcp.speed);
          PacketHandler_sendPacket(&ph, &mcp.h);
          uint8_t bytes = PacketHandler_txSize(&ph); 
          for(int i=0; i<bytes; i++) {
            uint8_t c = PacketHandler_writeByte(&ph);
            write(fds[MEGA], &c, 1);
          }
          printf("Packet SENT\n");
        }
        //sendPacket(fds[MEGA], buf, len);
        uint8_t num= 2;
        //write(fds[MEGA], &num, 1);
        
        
        printButton(e.number);
      }
      
      if(e.type == JS_EVENT_AXIS) {   // Verifying that an axis has been moved
        //mcp = alterPacket(&e);
        printf("Speed is %d \n",mcp.speed);
        
        /*
        int16_t val = MAPPED_VALUE(e.value);
        
        if(e.number == AXIS_R2) {
          printf("R2 with %d \n", val);
        }
        
        if(e.number == AXIS_L2) {
          printf("L2 with %d \n", val);
        }
        
        
        if(e.number == AXIS_LX || e.number == AXIS_RX) {
            if(val > 128) printf("RIGHT\n");
            if(val < 128) printf("LEFT\n");
        }
        if(e.number == AXIS_LY || e.number == AXIS_RY) {
            if(val > 128) printf("DOWN\n");
            if(val < 128) printf("UP\n");
        }
        */
      }
      
      //TODO: Send packet
      
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
  int ret, i, fds[NUM_DEVICES];

  /*
  int joy_fd = open("/dev/input/js1", O_RDONLY | O_NONBLOCK);  // Getting the file descriptor of the joystick on js1
  if(joy_fd < 0) {
    printf("Error %d opening %d.\n", errno, joy_fd);
    printf("Cannot find a joystick. Please connect one.\n");
    exit(1);
  } else fds[JOYSTICK] = joy_fd;
  */
  int mega_fd = setupSerial("/dev/ttyACM0", SPEED);
  if (mega_fd >= 0) fds[MEGA] = mega_fd;
  
  // If necessary insert here new file descriptors
  
  /*
  for (i=0; i<NUM_THREADS; ++i) {
    if(!i) ret = pthread_create(&threads[i], NULL, mainRoutine, (void*)fds); // Creating Thread#0 to manage a joystick
    if (ret != 0) {
      printf("ERROR: code %d\n", ret);
      exit(1);
    }
  }
  */

  PacketHandler ph;
  PacketHandler_init(&ph);
  int num_pkt = -1;
  while(1) {
    MotorControlPacket mcp;
    mcp.speed = num_pkt;
    mcp.h.id = ID_MOTOR_CONTROL_PACKET;
    mcp.h.size = sizeof(mcp);
    
    PacketHandler_sendPacket(&ph, &mcp.h);
    uint8_t bytes = PacketHandler_txSize(&ph); 
    for(int i=0; i<bytes; i++) {
      uint8_t c = PacketHandler_writeByte(&ph);
      write(fds[MEGA], &c, 1);
    }
    num_pkt++;
    printf("Packet SENT %d\n", num_pkt);
    
    usleep(800000);
    
  }
  /*
  printf("I'm the host, doing some stuff\n");
  int x = 0;
  while(x < 100000000) {
  	++x;
  }
  printf("Done!\n");
  */

  //pthread_exit(NULL);
  return 0;
}
