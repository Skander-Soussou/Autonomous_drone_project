#ifndef CRAZYFLIE_LOOP_FUNCTIONS_H
#define CRAZYFLIE_LOOP_FUNCTIONS_H

#include <argos3/core/simulator/loop_functions.h>
#include <controllers/crazyflie_sensing/crazyflie_sensing.h>
#include <argos3/core/simulator/simulator.h>
#include <sio_client.h>
#include <mutex>
#include <condition_variable>
#include <string>

#define TICKS_PER_SECOND 10
#define ROUTE "http://127.0.0.1:6001"

/**
 * Inspired from https://github.com/socketio/socket.io-client-cpp/blob/master/examples/Console/main.cpp 
*/
using namespace argos;

class CCrazyflieLoopFunctions : public CLoopFunctions{
public:
   CCrazyflieLoopFunctions();
   virtual ~CCrazyflieLoopFunctions();

   virtual void Init(TConfigurationNode &t_tree);
   virtual void Reset();
   virtual void Destroy();
   virtual void PostStep();
   virtual void PreStep();
   virtual void PostExperiment();
   
   void ConnectToServer();

   void setDronesState(MissionState newState);

   void transmitDroneData();

   void updateDroneCount();
   
   // void setDronesInitPosition();

private:

   // Binds each event name (Received through the socket ) to a callback
   void bind_events();

   sio::client client;

   std::mutex _lock;

   bool connect_established;

   std::condition_variable_any _cond;

   sio::socket::ptr current_socket;

   MissionState currentState;

   uint8_t droneCount;

};

#endif
