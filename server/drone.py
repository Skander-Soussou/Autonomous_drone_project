import time;
import cflib.crtp
from cflib.crazyflie import Crazyflie
from cflib.crazyflie.syncCrazyflie import SyncCrazyflie
from cflib.crazyflie.log import LogConfig
class Drone:

    def __init__(self,link_uri, drone_id, connected):
        self.link_uri = link_uri;
        self.drone_id = drone_id;
        self.connected = False;
        self.battery = 0;
        self.battery_level(self.link_uri);
        self.x = 0;
        self.y=0;
        self.range_front = 0;
        self.range_left = 0;
        self.range_right = 0;
        self.range_back = 0;
        
    def get_link_uri(self):
        return self.link_uri;
    
    def get_drone_id(self):
        return self.drone_id;
    
    def get_drone_connected(self):
        return self.connected;
    
    def set_drone_connected(self, is_connected):
        self.connected = is_connected;
        
    def get_battery_pourcent(self):
        return self.battery;
    
    def set_battery_pourcent(self, battery):
        self.battery = battery;

    def battery_callback(self, timestamp, data, logconf):
        if(type(data['pm.vbat']) != str):            
            self.battery = self.vbat_to_pourcentage(data['pm.vbat']);
            print("BAttery voltage: ", data['pm.vbat'])
    
    def logs(self, scf, logconf, cb):
        cf = scf.cf
        cf.log.add_config(logconf)
        logconf.data_received_cb.add_callback(cb);
        logconf.start()
        time.sleep(5)
        logconf.stop();
    
    def position_callback(self, timestamp, data, logconf):
        if(type(data['kalman.stateX']) != str):
            self.x = data['kalman.stateX']
            self.y = data['kalman.stateY']
            # self.range_front = data['range.front']
            # self.range_back= data['range.back']
            # self.range_left= data['range.left']
            # self.range_right= data['range.right']
            print("x: " + data['kalman.stateX'])
            print("y: " + data['kalman.stateY'])
            # print("front: " + data['range.front'])
            # print("back: " + data['range.back'])
            # print("left: " + data['range.left'])
            # print("right: " + data['range.right'])

    def drone_position(self, link_uri):
        cflib.crtp.init_drivers()
        log_pos = LogConfig(name='Position', period_in_ms=10)
        log_pos.add_variable("kalman.stateX", "float"),
        log_pos.add_variable("kalman.stateY", "float"),
        # log_pos.add_variable("kalman.stateZ", "float"),
        # log_pos.add_variable("range.front", "float"),
        # log_pos.add_variable("range.back", "float"),
        # log_pos.add_variable("range.left", "float"),
        # log_pos.add_variable("range.right", "float"),

        with SyncCrazyflie(self.link_uri, cf=Crazyflie(rw_cache='./cache')) as scf:
            self.logs(scf, log_pos, self.position_callback);


    def battery_level(self, link_uri):
        cflib.crtp.init_drivers()
        log_conf = LogConfig(name='Battery', period_in_ms=10)
        log_conf.add_variable('pm.vbat', 'float');
        with SyncCrazyflie(self.link_uri, cf=Crazyflie(rw_cache='./cache')) as scf:
                self.logs(scf, log_conf, self.battery_callback);
    
    def vbat_to_pourcentage(self, voltage):
        if 4.2 >= voltage > 4.15:
            return 100
        elif 4.15 >= voltage > 4.11:
            return 95
        elif 4.11 >= voltage > 4.08:
            return 90
        elif 4.08 >= voltage > 4.02:
            return 85
        elif 4.02 >= voltage > 3.98:
            return 80
        elif 3.98 >= voltage > 3.95:
            return 75
        elif 3.95 >= voltage > 3.91:
            return 70
        elif 3.91 >= voltage > 3.87:
            return 65
        elif 3.87 >= voltage > 3.85:
            return 60
        elif 3.85 >= voltage > 3.84:
            return 55
        elif 3.84 >= voltage > 3.82:
            return 50
        elif 3.82 >= voltage > 3.8:
            return 45
        elif 3.8 >= voltage > 3.79:
            return 40
        elif 3.79 >= voltage > 3.77:
            return 35
        elif 3.77 >= voltage > 3.75:
            return 30
        elif 3.75 >= voltage > 3.73:
            return 25
        elif 3.73 >= voltage > 3.71:
            return 20
        elif 3.71 >= voltage > 3.69:
            return 15       
        elif 3.69 >= voltage > 3.61:
            return 10
        elif 3.61 >= voltage > 3.27:
            return 5
        elif voltage < 3.27:
            return 0

