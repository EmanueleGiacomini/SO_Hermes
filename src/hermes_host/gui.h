#include <gtk/gtk.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>

#include "hermes_packets.h"
#include "print_packets.h"
#include "packet_handler.h"

void initGUI(int fd, uint8_t* end_flag, uint8_t* send_sem, PacketHandler* ph, MotorStatusPacket* motor_status);
void sendPacket(int fd, PacketHandler* ph, PacketHeader* h, uint8_t* sem);
