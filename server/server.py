from flask import Flask, request, Response, jsonify;
from flask_cors import cross_origin;
from  crazy_flie_connect import CrazyflieConnect;
import threading;
import os


def crazy_flie_connect():
    drone_physic = CrazyflieConnect();
    return drone_physic;

app = Flask(__name__);

@app.route('/start/connect', methods=['POST'])
@cross_origin()
def connect_drones():
    connect = request.json['type'];
    if(connect):
        drone_physic.connect_drones(connect);
        return Response(status=201);
    return Response(status=500);

@app.route('/type/mission', methods=['POST'])
@cross_origin() 
def type_mission():
    mission_type = request.json['type'];
    if(mission_type):
        drone_physic.simulation(int(mission_type));
        return Response(status=201)
    return Response(status=500);

@app.route('/finish/mission', methods=['POST'])
@cross_origin() 
def finish_mission():
    mission_type = request.json['type'];
    if(mission_type):
        drone_physic.stop_simulation();
        return Response(status=201)
    return Response(status=500);

@app.route('/start/identify/drone', methods=['POST'])
@cross_origin()
def indentify_drone():
    data = request.json['type'];
    uri = request.json['data'];
    if(data):
        drone_physic.identify_drone(int(data),uri);
        return Response(status=201);   
    return Response(status=500);

@app.route('/disconnect', methods=['POST'])
@cross_origin()
def disconnect_drones():
    mission_type = request.json['type'];
    if(mission_type):
        drone_physic.disconnect_drones();
        return Response(status=201)
    return Response(status=500);

@app.route('/runArgos', methods=['POST'])
def runArgos():
    # exec(open("/home/omar/Desktop/Project_3/Application-Web/server/socket1_argos.py").read())
    # exec(open("/home/omar/Desktop/Project_3/Application-Web/server/socket2_argos.py").read())
    os.system('sudo docker-compose build ; x11docker --user=RETAIN --network=host --workdir=/root/examples  --  -- simulation argos3 -c ./experiments/crazyflie_sensing.argos');
    return Response(status=201);

@app.route('/state/<data>')
@cross_origin()
def state(data):
    return jsonify(drone_physic.connected("radio://0/80/250K/E7E7E7E7" + data));

@app.route('/state/drone')
def state_drone():
    return jsonify(drone_physic.drone_state());


if __name__ == '__main__':
    drone_physic = crazy_flie_connect();
    app.run(port='5000',debug=True, host='0.0.0.0');
    