from logging import debug
import time
from cflib.crazyflie import Crazyflie
from cflib.crazyflie.log import LogConfig
from cflib.crazyflie.syncCrazyflie import SyncCrazyflie 
from flask_socketio import SocketIO, send, emit;
from flask import Flask;
from flask_cors import cross_origin;
import  threading;
import cflib.crtp
from flask_cors import CORS

app = Flask(__name__);
socketio = SocketIO(app)
socketio.init_app(app, cors_allowed_origins="*")
app.config['CORS_HEADERS'] = 'Content-Type'
CORS(app);


battery_pourctenge = 0;
position ={"x":0, "y":0};
radio_uri= ['radio://0/80/250K/E7E7E7E701']



            
    
def position_callback(timestamp, data, logconf):
    emit(data);

@socketio.on('message')
def drone_position():
    cflib.crtp.init_drivers()
    log_pos = LogConfig(name='Position', period_in_ms=10)
    log_pos.add_variable("kalman.stateX", "float"),
    log_pos.add_variable("kalman.stateY", "float"),
    log_pos.add_variable("kalman.stateZ", "float"),
    with SyncCrazyflie('radio://0/80/250K/E7E7E7E701', cf=Crazyflie(rw_cache='./cache')) as scf:
        logs(scf, log_pos, position_callback);

# @socketio.on('connect')
# def test():
#     send('yyyaaas')

def logs(scf, logconf, cb):
    cf = scf.cf
    cf.log.add_config(logconf)
    logconf.data_received_cb.add_callback(cb);
    logconf.start()
    while (1):
        i = 0
    logconf.stop();
    

if __name__ == '__main__':
    socketio.run(app, port=5033, debug = True)