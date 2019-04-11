/**
 * hermes_comm.h
 **/

#pragma once
#include "hermes_packets.h"
#include "packet_handler.h"
#include "packet_status.h"

#include "hermes_nRF24L01.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define O_UART 0x01
#define O_NRF24L01 0x02

void HermesComm_init(uint8_t);

PacketStatus HermesComm_sendPacket(PacketHeader*, uint8_t);

PacketStatus HermesComm_readPacket(PacketHeader*);

PacketStatus HermesComm_handle(void);


