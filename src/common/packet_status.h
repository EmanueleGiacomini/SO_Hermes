/**
 * packet_status.h
 **/
#pragma once

typedef enum {
  BufferEmtpy=-4,
  ChecksumError=-3,
  WrongSize=-2,
  UnknownType=-1,
  Success=0,
  ChecksumSuccess=1,
}PacketStatus;

