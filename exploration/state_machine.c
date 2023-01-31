#include "state_machine.h"

#define STATE_MACHINE_COMMANDER_PRI 3


void treatOrder(struct Drone *drone)
{
    switch(drone->command)
    {
        case IDENTIFIER:
            ledSetAll();
            break;
        case TAKE_OFF:
            drone->currentState = TAKE_OFF;
            drone->keep_flying = true;
            break;
        case LAND:
            drone->currentState = LANDING;
            drone->keep_flying = false;
            break;
        case START_MISSION:
            drone->currentState = TAKE_OFF;
            drone->keep_flying = true;
            break;
        case RETURN_TO_BASE:
            drone->currentState = RETURN_TO_BASE;
            drone->keep_flying = false;
            break;
        default:
            break;
    }
}

void step(struct Drone *drone, struct Sensors *sensor, struct Rssi *rssi)
{
    if(drone->keep_flying)
    {
        if(drone->taken_off)
        {
            drone->vel_w_cmd = 0;
            hover(&drone->setpoint_BG, drone->nominal_height);
            if (rssi->id_inter_closest > drone->my_id) {
                rssi->rssi_inter_filtered = 140;
            }
            drone->state = wall_follower_and_avoid_controller(&drone->vel_x_cmd, &drone->vel_y_cmd, &drone->vel_w_cmd, sensor->front_range, sensor->left_range, sensor->right_range,sensor->heading_rad, rssi->rssi_inter_filtered);
            float vel_w_cmd_convert = drone->vel_w_cmd * 180.0f / (float)M_PI;
            vel_command(&drone->setpoint_BG, drone->vel_x_cmd, drone->vel_y_cmd, vel_w_cmd_convert, drone->nominal_height);
            drone->on_the_ground = false;
        }
        else
        {
            if (usecTimestamp() >= drone->takeoffdelaytime + 1000*1000*drone->my_id) {
                take_off(&drone->setpoint_BG, drone->nominal_height);
                if (sensor->height > drone->nominal_height) {
                    drone->taken_off = true;
                    if (drone->my_id%2==1)
                        init_wall_follower_and_avoid_controller(0.4, 0.5, -1);
                    else
                        init_wall_follower_and_avoid_controller(0.4, 0.5, 1);
                }
                drone->on_the_ground = false;
            }
            else
            {
                shut_off_engines(&drone->setpoint_BG);
                drone->taken_off = false;
            }
        }
    
    }
    else
    {
        if(drone->taken_off)
        {
            land(&drone->setpoint_BG, 0.2f);
            if (sensor->height < 0.1f) {
                shut_off_engines(&drone->setpoint_BG);
                drone->taken_off = false;
            }
            drone->on_the_ground = false;
            }
        else
        {
            shut_off_engines(&drone->setpoint_BG);
            drone->takeoffdelaytime=usecTimestamp();
            drone->on_the_ground = true;
        }
    }
    if (usecTimestamp() >= drone->radioSendBroadcastTime + 1000*500) {
        radiolinkSendP2PPacketBroadcast(&drone->p_reply);
        drone->radioSendBroadcastTime = usecTimestamp();
    }
    commanderSetSetpoint(&drone->setpoint_BG, STATE_MACHINE_COMMANDER_PRI);

}

void updateDrone(struct Rssi *rssi, struct Drone *drone, struct Sensors *sensor)
{
    for (uint8_t it = 0; it < 9; it++) if (usecTimestamp() >= rssi->time_array_other_drones[it] + 1000*1000) {
        rssi->time_array_other_drones[it] = usecTimestamp() + 1000*1000+1;
        rssi->rssi_array_other_drones[it] = 150;
        rssi->rssi_angle_array_other_drones[it] = 500.0f;
    }
    rssi->id_inter_closest = (uint8_t)find_minimum(rssi->rssi_array_other_drones, 9);
    rssi->rssi_inter_closest = rssi->rssi_array_other_drones[rssi->id_inter_closest];
    rssi->rssi_angle_inter_closest = rssi->rssi_angle_array_other_drones[rssi->id_inter_closest];
    rssi->rssi_inter_filtered =  (uint8_t)update_median_filter_f(&drone->medFilt_2, (float)rssi->rssi_inter_closest);
    paramVarId_t varid = paramGetVarId("deck", "bcMultiranger");
    uint8_t multiranger_isinit=paramGetInt(varid);
    varid = paramGetVarId("deck", "bcFlow2");
    uint8_t flowdeck_isinit=paramGetUint(varid);
    logVarId_t logvarid = logGetVarId("kalman", "stateZ");
    sensor->height = logGetFloat(logvarid);
    logvarid = logGetVarId("stabilizer", "yaw");
    float heading_deg = logGetFloat(logvarid);
    sensor->heading_rad = heading_deg * (float)M_PI / 180.0f;
    logvarid = logGetVarId("radio", "rssi");
    rssi->rssi_beacon = logGetFloat(logvarid);
    rssi->rssi_beacon_filtered =  (uint8_t)update_median_filter_f(&drone->medFilt_3, (float)rssi->rssi_beacon);
    if (multiranger_isinit) {
        sensor->front_range = (float)rangeGet(rangeFront) / 1000.0f;
        sensor->right_range = (float)rangeGet(rangeRight) / 1000.0f;
        sensor->left_range = (float)rangeGet(rangeLeft) / 1000.0f;
        sensor->back_range = (float)rangeGet(rangeBack) / 1000.0f;
        sensor->up_range = (float)rangeGet(rangeUp) / 1000.0f;
    }
    estimatorKalmanGetEstimatedPos(&drone->pos);
    memset(&drone->setpoint_BG, 0, sizeof(drone->setpoint_BG));
    sensor->up_range_filtered = update_median_filter_f(&drone->medFilt, sensor->up_range);
    if (sensor->up_range_filtered < 0.05f) {
        sensor->up_range_filtered = sensor->up_range;
    }
    if (flowdeck_isinit && multiranger_isinit ) {
      drone->correctly_initialized = true;
    }
    if (drone->keep_flying == true && (!drone->correctly_initialized || sensor->up_range < 0.2f)) {
        drone->keep_flying = 0;
    }
    drone->state = 0;
}

int32_t find_minimum(uint8_t a[], int32_t n)
{
  int32_t c, min, index;

  min = a[0];
  index = 0;

  for (c = 1; c < n; c++) {
    if (a[c] < min) {
      index = c;
      min = a[c];
    }
  }

  return index;
}