#ifndef DRONE_H
#define DRONE_H

#include "commander.h"
#include "radiolink.h"
#include "median_filter.h"

#endif
typedef enum
{
  TAKE_OFF,
  LAND,
  IDENTIFIER,
  START_MISSION,
  RETURN_TO_BASE,
  DO_NOTHING,
} Command;

typedef enum
{
  NOT_READY,
  READY,
  HOVERING,
  TAKING_OFF,
  EXPLORATING,
  LANDING,
  RETURNING_TO_BASE,
  CRASHED,
} State;

struct Drone
{
    uint8_t my_id;
    bool keep_flying;
    bool taken_off;
    bool on_the_ground;
    bool correctly_initialized;
    bool outboud;
    float vel_x_cmd;
    float vel_y_cmd;
    float vel_w_cmd;
    int state;
    int state_wf;
    float nominal_height;
    setpoint_t setpoint_BG;
    P2PPacket p_reply;
    uint64_t takeoffdelaytime;
    uint64_t radioSendBroadcastTime;
    point_t pos;
    Command command;
    State currentState;
    struct MedianFilterFloat medFilt;
    struct MedianFilterFloat medFilt_2;
    struct MedianFilterFloat medFilt_3;
};

struct Rssi
{
    uint8_t rssi_inter;
    uint8_t rssi_inter_filtered;
    uint8_t rssi_inter_closest;
    float rssi_angle_inter_ext;
    float rssi_angle_inter_closest;
    uint8_t rssi_beacon;
    uint8_t rssi_beacon_filtered;
    uint8_t id_inter_ext;
    uint8_t rssi_array_other_drones[9];
    uint64_t time_array_other_drones[9];
    float rssi_angle_array_other_drones[9];
    uint8_t id_inter_closest;
    float rssi_angle;

};

struct Sensors
{
    float heading_rad;
    float right_range;
    float front_range;
    float left_range;
    float up_range;
    float back_range;
    float up_range_filtered;
    float height;
};

