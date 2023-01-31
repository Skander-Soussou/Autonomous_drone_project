#include "mainapp.h"

struct Drone drone;
struct Rssi rssi;
struct Sensors sensor;
struct PacketRX rx_packet;
struct testPacketTX tx_packet;


void appMain(void *param)
{
    initDrone();
    systemWaitStart();
    vTaskDelay(M2T(3000));

    while (1)
    {
        updateDrone(&rssi,&drone,&sensor);
        treatOrder(&drone);
        step(&drone,&sensor,&rssi);
        if (appchannelReceivePacket(&rx_packet, sizeof(rx_packet), 0))
        {
            drone.command = rx_packet.order;
            tx_packet.test = drone.command;
            appchannelSendPacket(&tx_packet, sizeof(tx_packet));
        }
    }
}

void p2pcallbackHandler(P2PPacket *p)
{
    rssi.id_inter_ext = p->data[0];
    if (rssi.id_inter_ext == 0x63)
    {
        // rssi_beacon =rssi_inter;
        drone.keep_flying =  p->data[1];
    }else if(rssi.id_inter_ext == 0x64){
        rssi.rssi_beacon =p->rssi;

    }
    else{
        rssi.rssi_inter = p->rssi;
        memcpy(&rssi.rssi_angle_inter_ext, &p->data[1], sizeof(float));

        rssi.rssi_array_other_drones[rssi.id_inter_ext] = rssi.rssi_inter;
        rssi.time_array_other_drones[rssi.id_inter_ext] = usecTimestamp();
        rssi.rssi_angle_array_other_drones[rssi.id_inter_ext] = rssi.rssi_angle_inter_ext;


    }
}

void initDrone()
{
    for(int i=0; i<9; i++)
    {
        rssi.rssi_angle_array_other_drones[i] = 150;
        rssi.time_array_other_drones[i] = 0;
        rssi.rssi_angle_array_other_drones[i] = 500.0f;
    }
    rssi.id_inter_closest = 100;
    init_median_filter_f(&drone.medFilt, 5);
    init_median_filter_f(&drone.medFilt_2, 5);
    init_median_filter_f(&drone.medFilt_3, 13);
    p2pRegisterCB(p2pcallbackHandler);
    uint64_t address = configblockGetRadioAddress();
    drone.my_id =(uint8_t)((address) & 0x00000000ff);
    drone.p_reply.port=0x00;
    drone.p_reply.data[0]=drone.my_id;
    memcpy(&drone.p_reply.data[1], &rssi.rssi_angle, sizeof(float));
    drone.p_reply.size=5;
    drone.takeoffdelaytime = 0;
    drone.command = DO_NOTHING;
    drone.currentState = NOT_READY;
    drone.keep_flying = false;
    drone.taken_off = false;
    drone.on_the_ground = true;
    drone.nominal_height = 0.3;
    drone.correctly_initialized=false;
}