/**
 * packet_status.h
 **/
#pragma once

typedef enum {
  ChecksumError=-3,
  WrongSize=-2,
  UnknownType=-1,
  Success=0
}PacketStatus;

