#ifndef MAIN_H
#define MAIN_H

#include <string.h>
#include <errno.h>
#include <math.h>

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "system.h"

#include "sensors.h"
#include "stabilizer_types.h"

#include "communication.h"
#include "state_machine.h"

// #include "SGBA.h"
#include "usec_time.h"


#include "configblock.h"
#endif

void p2pcallbackHandler(P2PPacket *p);
void initDrone();

