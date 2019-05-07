#include "hermes_host.h"
#include "serial.h"
#include "joystick.h"
#include "gui.h"

char buf[256];
uint8_t end_flag = 0;
uint8_t send_sem = 1;
PacketHandler ph;

void recvFn(PacketHeader* recvp, void* _args) {
  PacketHeader* dest=(PacketHeader*)_args;
  memcpy(dest, recvp, dest->size);
  PrintPacket(dest, buf);
  printf("\33[2K");
  printf("%s\r", buf);
  fflush(stdout);
}

MotorStatusPacket motor_status={
  {
    .id=ID_MOTOR_STATUS_PACKET,
    .size=sizeof(motor_status),
    .seq=0,
    .dest_addr=0,
    .src_addr=0,
    .checksum=0,
  },
  .encoder_ticks=0,
  .desired_speed=0,
  .measured_speed=0,
};

PacketOperation motor_status_op={
  .id=ID_MOTOR_STATUS_PACKET,
  .size=sizeof(motor_status),
  .on_receive_fn=recvFn,
  .args=(void*)&motor_status
};


void* statusRoutine(void *arg) {
  
  int* fds = (int*)arg;
  uint8_t c;
  while(!end_flag) {
    int n=read(fds[MEGA], &c, 1);
    if(n) {
      PacketStatus status=PacketHandler_readByte(&ph, c);
    }
  }
  pthread_exit(NULL);
}

void* mainRoutine(void *arg) {
  int* fds = (int*)arg;
  int endit = 0, i = 0;
  js_event e;

  printf("-------------------------------\n");
  printf("Welcome to HERMES!\n");
  printf("-------------------------------\n");

  MotorControlPacket mcp = {
    {
      .id=ID_MOTOR_CONTROL_PACKET,
      .size=sizeof(mcp),
      .seq=0,
      .dest_addr=0,
      .src_addr=0,
      .checksum=0,
    },
    .mode=1,
    .speed=0,
  };

  while(1) {
    int reade = read(fds[JOYSTICK], &e, sizeof(e));
    if(reade > 0) {
    //while(reade > 0) {
      //MotorControlPacket mcp;
      if(e.type == JS_EVENT_BUTTON && e.value == BTN_PRES) { 	// Verifying that a button has been pressed
        //printf("fuck\n");
        
        if(e.number == BTN_PS) {
          endit = 1;
          break;
        }
        
        if(!alterPacket(&e, &mcp)) sendPacket(fds[MEGA], &ph, (PacketHeader*)&mcp, &send_sem);
        
        /* test print
        printf("time -> %d \n", e.time);
        printf("value -> %d \n", e.value);
        printf("type -> %d \n", e.type);
        printf("number -> %d \n", e.number);
        */
        
      }
      
      /*
      if(e.type == JS_EVENT_AXIS) {   // Verifying that an axis has been moved
        
        if(e.number == AXIS_LX || e.number == AXIS_RX) {
            if(val > 128) printf("RIGHT\n");
            if(val < 128) printf("LEFT\n");
        }
        if(e.number == AXIS_LY || e.number == AXIS_RY) {
            if(val > 128) printf("DOWN\n");
            if(val < 128) printf("UP\n");
        }
        
        if(!alterPacket(&e, &mcp)) sendPacket(fds[MEGA], &ph, &mcp.h);
        
      }
      */
      
      //printf("\33[2K\rSpeed is %d",mcp.speed);
      //fflush(stdout);
    }
    
    if(endit) {
      end_flag = 1;
      break;
    }
    
    /*
    if(errno != EAGAIN) {
      printf("An error occured while reading the event.\n");
      printf("Error code: %d\n", errno);
      pthread_exit(NULL);
    }
    */
    
  }

  for(i=0; i<NUM_DEVICES; ++i) {  // Closing all devices
    if(close(fds[i])) {
      printf("An error occured while closing the file descriptor %d.\n", fds[i]);
      printf("Error code: %d\n", errno);
      pthread_exit(NULL);
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
  
  if(argc < 4) {
    printf("ERROR: Method usage: ./hermes_host.elf <serial-dev> <js-dev> <gui-enable>\n");
    exit(1);
  }
  
  
  
  int joy_fd = open(argv[2], O_RDONLY);  // Getting the file descriptor of the joystick on js1, eventually O_NONBLOCK
  if(joy_fd < 0) {
    printf("Error %d opening %d.\n", errno, joy_fd);
    printf("Cannot find a joystick. Please connect one.\n");
    exit(1);
  } else fds[JOYSTICK] = joy_fd;
  
  int mega_fd = setupSerial(argv[1], SPEED);
  if (mega_fd >= 0) fds[MEGA] = mega_fd;
  
  // If necessary insert here new file descriptors //

  PacketHandler_init(&ph);
  PacketHandler_addOperation(&ph, &motor_status_op);
  
  for (i=0; i<NUM_THREADS; ++i) {
    if(!i) ret = pthread_create(&threads[i], NULL, mainRoutine, (void*)fds); // Creating Thread#0 to manage a joystick
    else ret = pthread_create(&threads[i], NULL, statusRoutine, (void*)fds);
    
    if (ret != 0) {
      printf("ERROR: code %d\n", ret);
      exit(1);
    }
  }
  
  // Check GUI enabled
  if(atoi(argv[3]) == 1) initGUI(fds[MEGA], &end_flag, &send_sem, &ph, &motor_status);
  
  //pthread_exit(NULL);
  for(i=0; i<NUM_THREADS; ++i) pthread_join(threads[i], NULL);
  
  return 0;
}
