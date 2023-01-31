import  pytest
import sys 
import os
sys.path.append(os.path.abspath("../"))
from drone import Drone;
from crazy_flie_connect import  CrazyflieConnect;

crazyflieConnect = CrazyflieConnect();
link_uri = crazyflieConnect.scan_interfaces();
drone_id = 0;
drone = Drone(link_uri[0][0], drone_id, False);

def test_get_drone_link_url():
    global drone;   
    assert drone.get_link_uri() == link_uri[0][0];
    
def test_get_drone_id():
    global drone;
    assert drone.get_drone_id() == drone_id;
    
def test_get_drone_connected():
    global drone;
    assert drone.get_drone_connected() == False;
    
def test_set_drone_connected():
    global drone;
    drone.set_drone_connected(True);
    assert drone.get_drone_connected() == True;