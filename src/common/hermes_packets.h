/**
 * hermes_packets.h
 **/

typedef struct {
  uint8_t seq;
  uint8_t id;
  uint8_t size;
  uint8_t dest_addr;
  uint8_t src_addr;
  uint16_t checksum;
} PacketHeader;

typedef struct {
  PacketHeader h;
  uint8_t mode;
  uint8_t speed;
} MotorControlPacket;
#define ID_MOTOR_CONTROL_PACKET 1

typedef struct {
  PacketHeader h;
  int16_t encoder_ticks;
  int16_t desidered_speed;
  int16_t measured_speed;
} MotorStatusPacket;
#define ID_MOTOR_STATUS_PACKET 2
