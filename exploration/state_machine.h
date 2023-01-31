#ifndef STATE_H 
#define STATE__H

#include <math.h>
#include "drone.h"
#include "range.h"
#include "led.h"
#include "log.h"
#include "param.h"
#include "setpoint.h"
#include "wallfollowing_with_avoid.h"
#include "estimator_kalman.h"

#endif

void treatOrder(struct Drone *drone);
void step(struct Drone *drone, struct Sensors *sensor, struct Rssi *rssi);
void updateDrone(struct Rssi *rssi, struct Drone *drone, struct Sensors *sensor);
int32_t find_minimum(uint8_t a[], int32_t n);