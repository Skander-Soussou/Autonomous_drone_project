# Source : https://python-socketio.readthedocs.io/en/latest/server.html
from logging import debug
from re import match
import time
from typing import Match
from flask import Flask, render_template
from flask_cors import cross_origin
from flask_socketio import _SocketIOMiddleware, SocketIO, emit

from enum import Enum

class ArgosStatus:
    OFF = "OFF"
    ON  = "ON"

simulationStatus = ArgosStatus.OFF
# The Socket.IO protocol supports multiple logical connections, all multiplexed on the same physical connection. Clients can open multiple connections by specifying a different namespace on each. A namespace is given by the client as a pathname following the hostname and port. For example, connecting to http: // example.com: 8000/chat would open a connection to the namespace / chat.

app = Flask(__name__)
socketio = SocketIO(app)

# The Socket.IO protocol is event based.
# When a client wants to communicate with the server it emits an event.
# Each event has a name, and a list of arguments.

@socketio.on("connect")
# sio.emit('my event', {'data': 'foobar'}, room=user_sid) to send an event to a particular client
# def connect(sid, data):
def connect(sid):
    # session ID : every event for thsi client, sid is the first parameter for any event
    # each client has a unique session ID
    # data : dictionnary with all the details about the client request
    # serverCommand = 0
    # print("Argos just connected: ")
    # print('Argos is ON ')
    # print('COMMAND LIST TO SEND TO ARGOS :')
    # print('***** 1 ****** : start mission')
    # print('***** 2 ****** : stop mission ')
    simulationStatus = ArgosStatus.ON
    print('Argos is currently ', simulationStatus)
    emit('message','testString')


@socketio.on('Agros Response event')
def print_message(data):
    # When we receive a new event of type
    # 'message' through a socket.io connection
    # we print the socket ID and the message

    # print(data)
    # print_message(data)
    # emit('message', {
    #     'data': 'Hello Argos ! '
    # })
    print("received message : ", data)


@socketio.on('CommandRequest')
def request():
    while(True):
        command = input("Argos is requesting a command please : ")
        command = int(command)
        # are there really no switch cases in python ??????
        if command == 1:
            print("Sending a 1 TAKEOFF")
            emit('TAKEOFF', "Take Off ")
        elif command == 2:
            print("Sending a 2 EXPLORE")
            emit('EXPLORATION', "EXPLORE")
        elif command == 3:
            print("Sending a 3 LAND")
            emit('LAND', "Finish the MISSION")
        elif command == 4:
            x = int(input("Input the x and orientation now"))
            y = int(input("Input the y and orientation now"))
            z=int(input("Input the z and orientation now"))
            orientation=int(input("Input the orientation now"))
            
            position = {'x':x,
                        'y':y,
                        'z':z,
                        'orientation':orientation}
            emit("setPosition", position)


@socketio.on('dataTransmission')
def dataTransmissionHandler(data):
    print(data)

@socketio.event
def disconnect():
    print("\nATTENTION : Argos disconnected")

# Handles the default namespace
@socketio.on_error()  
def error_handler(e):
    print('error')
    pass


@socketio.on('CommandAck')
def AckReceived():
    # are there really no switch cases in python ??????
    print("Ack received")

## If we wanted to create a new websocket endpoint,
## use this decorator, passing in the name of the
## event we wish to listen out for

if __name__ == '__main__':
    socketio.run(app, host='127.0.0.1', port='6001', debug=True)
    
# print("Emitting command")
# socketio.emit('startMission', {'data': 'foobar'})
# while 1:
#     print("Emitting command")
#     socketio.emit('startMission', {'data': 'foobar'})
#     time.sleep(5)

