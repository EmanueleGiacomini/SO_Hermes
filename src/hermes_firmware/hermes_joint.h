/**
 * hermes_joint.h
 **/

#pragma once
#include <stdint.h>
#include "hermes_packets.h"
#include "hermes_joint_internal.h"

#define NUM_JOINTS 1

void HermesJoints_init(void);
void HermesJoints_handle(void);
