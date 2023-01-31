import logging
import time
from threading import Thread
import struct
import cflib
from cflib.crazyflie import Crazyflie
from drone import Drone
import cflib.crtp
import logging;
from enum import Enum
from cflib.utils import uri_helper
from flask import jsonify
import drone_info;
import threading
class CrazyflieConnect:
    class State_Drone(Enum):
        MISSION_END = 0;
        MISSION_START = 1;
        LED = 2;    
    
    def __init__(self):
        cflib.crtp.init_drivers(enable_debug_driver=False)
        #self.scan_interfaces();
        self.connected_drones = []
        self.drone = [];
        self.mission = None;
        for index, item in enumerate(drone_info.radio_uri):
            self._cf = Crazyflie();
            self._cf.connected.add_callback(self.connection_success)
            self._cf.disconnected.add_callback(self.disconnected)
            self._cf.connection_failed.add_callback(self.connection_failed)
            self._cf.connection_lost.add_callback(self.connection_lost)
            self._cf.appchannel.packet_received.add_callback(self.receive_message)
            self.URI = uri_helper.uri_from_env(default=item)
            self.drone.append(Drone(self.URI, 0, False));
            self.connected_drones.append([self._cf,self.drone[index]]);
            print(item);
    

    def scan_interfaces(self):
        while True:
            available_drones = cflib.crtp.scan_interfaces()
            if len(available_drones) > 0:
                print(available_drones);
                break;
            else:
                print("We didn't have found any drone yet, we will try after 5s");
                time.sleep(5);

    
    def connect_drones(self,data):
        for i in range(len(self.connected_drones)):
                if(self.connected_drones[i][1].get_drone_connected() == False and self.connected_drones[i][1].get_link_uri() == data):
                    print('data',data);
                    self.connected_drones[i][0].open_link(data);
                    break;
        
        
        #drone_logs.drone(self.connected_drones[0][1].get_link_uri());
        #self.connected_drones[0][1].set_battery_pourcent(drone_logs.battery_pourctenge)
        #print('battery', self.connected_drones[0][1].get_battery_pourcent())

    def disconnect_drones(self):
        for i in range(len(self.connected_drones)):
            self.connected_drones[i][0].close_link();
            self.connected_drones[i][1].set_drone_connected(False);
            
    
    def simulation(self, mission):
        #drone(self.connected_drones[0][1].get_link_uri());
        self.mission = mission;
        self.send_message(mission);
        # for i in range(len(self.connected_drones)):
        #self.connected_drones[0][1].drone_position(self.connected_drones[0][1].get_link_uri());
        
    
    def identify_drone(self, data, uri):
        for i in range(len(self.connected_drones)):
            if(uri == self.connected_drones[i][1].get_link_uri()):
                self.send_message(data);
                break;
    
    def stop_simulation(self):
        self.mission = 1;
        self.send_message(1)
    
    def connected(self, link_uri):
        for i in range(len(self.connected_drones)):
            if(self.connected_drones[i][1].get_drone_connected() == False and link_uri == self.connected_drones[i][1].get_link_uri()):
                self.connected_drones[i][1].set_drone_connected(True);
                print('Connection to : %s success' %(link_uri));
                return {link_uri : 'en attente',  'battery':self.connected_drones[i][1].get_battery_pourcent()};
            
    def connection_success(self, link_uri):
        self.connected(link_uri);
    
    def connection_failed(self, link_uri, msg):
        print('Connection to %s failed: %s' % (link_uri, msg))
        

    def connection_lost(self, link_uri, msg):
        print('Connection to %s lost: %s' % (link_uri, msg))

    def disconnected(self, link_uri):
        print('Disconnected from %s' % link_uri)

    def receive_message(self, data):
        (data, ) = struct.unpack("<B", data)
        print(f"Received data:", data)
        return data;

    def send_message(self, state):
            data = struct.pack("<B", state)
            for i in range(len(self.connected_drones)):
                if (self.connected_drones[i][1].get_drone_connected()):
                    self.connected_drones[i][0].appchannel.send_packet(data);
                    print(f"Sent data : {data}")
    
    def drone_state(self):
        
        if(self.mission == 0):
            return "En mission"
        elif(self.mission == 1):
            return "Mission Finie"
        else:
            return "En attente";         
                
            
