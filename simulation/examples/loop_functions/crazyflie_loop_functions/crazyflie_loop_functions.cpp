#include "crazyflie_loop_functions.h"
#include <argos3/plugins/robots/crazyflie/simulator/crazyflie_entity.h>
#include <argos3/core/utility/configuration/argos_configuration.h>
#include <argos3/core/utility/logging/argos_colored_text.h>
#include <argos3/core/utility/math/vector3.h>

CCrazyflieLoopFunctions::CCrazyflieLoopFunctions() : 
                        connect_established(false), 
                        currentState(WAITING_INSTRUCTIONS), 
                        droneCount(0){}

CCrazyflieLoopFunctions::~CCrazyflieLoopFunctions(){

}

/****************************************/
/****************************************/

/**
 * An XML document (like the .argos in /experiments ) is an ordered,
 *  labeled tree. Each node of the tree is an XML element and 
 * is written with an opening and closing tag . 
 * An element can have one or more XML attribute.
 * Here, t_tree represents the loop_functions node in the .argos file
*/
void CCrazyflieLoopFunctions::Init(TConfigurationNode &t_tree){
   ConnectToServer();
   updateDroneCount();
};

void CCrazyflieLoopFunctions::Reset(){

   std::cout << "Resetting drones !" << std::endl;

   setDronesState(WAITING_INSTRUCTIONS);
};

void CCrazyflieLoopFunctions::Destroy(){
   std::cout << "Ending simulation " << std::endl;

   client.sync_close();
   client.clear_socket_listeners();
   // ! Try with this later ???
   client.clear_con_listeners();
};

/* void CCrazyflieLoopFunctions::PostExperiment(){
};
 */

void CCrazyflieLoopFunctions::ConnectToServer(){
   // Called when connection is closed or dropped
   client.set_fail_listener([&](){
                                    // on fail
                                    std::cout << " Socket IO : Failed to reach the server. Please checkout if server is running. " << std::endl;
                                    client.close();
                                    connect_established = false; });

   // Called when connection is closed or dropped
   client.set_socket_close_listener([&](std::string const &nsp){
                                       std::cout << "Socket IO : Socket closed " << std::endl;
                                       connect_established = false;
                                    });

   client.set_socket_open_listener([&](std::string const &nsp){
                                      // On connected with the server
                                      _lock.lock();
                                      _cond.notify_all();
                                      connect_established = true;
                                      _lock.unlock();

                                      // ! All designated constructor of message objects is hidden, you need to create message and get the message::ptr by [derived]_message:create().
                                      std::cout << green << "Connection established"  << std::endl <<
                                       "Now requesting a command" << std::endl << 
                                      reset ;
                                      current_socket->emit("CommandRequest");
                                   });
                                   
   try{
      client.connect(ROUTE);
   }catch (CARGoSException &ex){
      _lock.lock();
      if (!connect_established){
         std::cout << "Establishing connection ... " << std::endl;
         _cond.wait(_lock);
      }
      _lock.unlock();
   }; 
   current_socket = client.socket();

   bind_events();
}

void CCrazyflieLoopFunctions::bind_events(){
   /****************************************************/
   current_socket->on("WAITING_INSTRUCTIONS", sio::socket::event_listener([&](sio::event &event)
                                                                          {
      _lock.lock();
      std::cout << "Command received from server" << std::endl;
      std::cout << red << "Returning to base" << reset << std::endl;
      setDronesState(MissionState::WAITING_INSTRUCTIONS); 
      _lock.unlock(); 
      }));

   current_socket->on("RETURN", sio::socket::event_listener([&](sio::event &event){
      _lock.lock();
      std::cout << "Command received from server: "  << std::endl;
      std::cout << red << "Returning to base" << reset << std::endl;
      setDronesState(MissionState::RETURN);
      _lock.unlock();
      //! When take off finishes, set state to exploration
   }));

   current_socket->on("TAKEOFF", sio::socket::event_listener([&](sio::event &event){
      _lock.lock();
      std::cout << "Command received from server: " << std::endl;
      std::cout << red << "TAKING OFF" << reset << std::endl;
      setDronesState(MissionState::TAKEOFF);
      current_socket->emit("CommandAck");
      //! When take off finishes, set state to exploration
      _lock.unlock(); 
   }));
   
   current_socket->on("LAND", sio::socket::event_listener([&](sio::event &event){
      _lock.lock();
      std::string eventMessage = event.get_message()->get_string();
      std::cout << "Command received from server: " << eventMessage << std::endl;
      std::cout << red << "LANDING DRONES" << reset << std::endl; 
      setDronesState(MissionState::LAND);
      current_socket->emit("CommandAck");
      _lock.unlock();
   }));

   current_socket->on("EXPLORE", sio::socket::event_listener([&](sio::event &event){
      _lock.lock();
      std::string eventMessage = event.get_message()->get_string();
      std::cout << "Command received from server: " << eventMessage << std::endl;
      std::cout << red << "LANDING DRONES" << reset << std::endl;       
      setDronesState(MissionState::EXPLORATION);
      current_socket->emit("CommandAck"); 
      _lock.unlock(); 
   }));

   current_socket->on("setPosition", sio::socket::event_listener([&](sio::event &event){
      int x = event.get_message()->get_map()["x"]->get_int();
      int y = event.get_message()->get_map()["y"]->get_int();
      int z = event.get_message()->get_map()["z"]->get_int();
      int orientation = event.get_message()->get_map()["orientation"]->get_int();

      std::cout << "Data received from server: (" << x << ", " << y << ", " << z << "), orientation : " << orientation << std::endl;
      std::cout << red << "Position" << reset << std::endl; 
      // setDronesState(MissionState::EXPLORATION);
      current_socket->emit("CommandAck"); 
   }));
}  

/****************************************************/

void CCrazyflieLoopFunctions::setDronesState(MissionState newState)
{
   CSpace::TMapPerType &m_cCrazyflies = GetSpace().GetEntitiesByType("crazyflie");
   for (CSpace::TMapPerType::iterator it = m_cCrazyflies.begin();
        it != m_cCrazyflies.end();
        ++it){
      /* Get handle to Crazyflie entity and controller */
      CCrazyflieEntity &cCrazyflie = *any_cast<CCrazyflieEntity *>(it->second);
      CCrazyflieSensing &cController = dynamic_cast<CCrazyflieSensing &>(cCrazyflie.GetControllableEntity().GetController());
      cController.setDroneState(newState);
   }
   currentState = newState;
}

// void CCrazyflieLoopFunctions::setDroneInitPosition(x,y,z,angle){
//    // Je vais translater tous les drones a la position specifie
//    CSpace::TMapPerType &m_cCrazyflies = GetSpace().GetEntitiesByType("crazyflie");
//    for (CSpace::TMapPerType::iterator it = m_cCrazyflies.begin();
//         it != m_cCrazyflies.end();
//         ++it)
//    {
//       /* Get handle to Crazyflie entity and controller */
//       CCrazyflieEntity &cCrazyflie = *any_cast<CCrazyflieEntity *>(it->second);
//       CCrazyflieSensing &cController = dynamic_cast<CCrazyflieSensing &>(cCrazyflie.GetControllableEntity().GetController());
//       MoveEntity();
//    }

// }
/****************************************************/

void CCrazyflieLoopFunctions::transmitDroneData(){
   sio::message::ptr message = sio::object_message::create();
   sio::message::ptr data    = sio::object_message::create();

   CSpace::TMapPerType &m_cCrazyflies = GetSpace().GetEntitiesByType("crazyflie");
   for (CSpace::TMapPerType::iterator it = m_cCrazyflies.begin();
        it != m_cCrazyflies.end();
        ++it){
      /* Get handle to Crazyflie entity and controller */
      CCrazyflieEntity &cCrazyflie = *any_cast<CCrazyflieEntity *>(it->second);
      CCrazyflieSensing &cController = dynamic_cast<CCrazyflieSensing &>(cCrazyflie.GetControllableEntity().GetController());

      argos::CVector3 position = cController.getPosition();
      argos::Real availableCharge = cController.getBattery();
      std::vector<argos::Real> ranges = cController.getDistanceScannerReading();

      data->get_map().insert(std::pair<std::string, sio::message::ptr>("ID", sio::string_message::create(cController.GetId())));
      data->get_map().insert(std::pair<std::string, sio::message::ptr>("State", sio::int_message::create(cController.getDroneState())));
      data->get_map().insert(std::pair<std::string, sio::message::ptr>("x", sio::double_message::create(position.GetX())));
      data->get_map().insert(std::pair<std::string, sio::message::ptr>("y", sio::double_message::create(position.GetY())));
      data->get_map().insert(std::pair<std::string, sio::message::ptr>("z", sio::double_message::create(position.GetZ())));
      data->get_map().insert(std::pair<std::string, sio::message::ptr>("BatteryLeft", sio::double_message::create(availableCharge)));
      data->get_map().insert(std::pair<std::string, sio::message::ptr>("Front", sio::double_message::create(ranges[0])));
      data->get_map().insert(std::pair<std::string, sio::message::ptr>("Left",  sio::double_message::create(ranges[1])));
      data->get_map().insert(std::pair<std::string, sio::message::ptr>("Back",  sio::double_message::create(ranges[2])));
      data->get_map().insert(std::pair<std::string, sio::message::ptr>("Right", sio::double_message::create(ranges[3])));

      message->get_map().insert(std::pair<std::string, sio::message::ptr>(std::to_string(cController.getCurrentDroneNumber()), data));
   }

   current_socket->emit("dataTransmission", message);
}

void CCrazyflieLoopFunctions::PostExperiment()
{
   std::cout << "Simulation ended"<< std::endl;
}

void CCrazyflieLoopFunctions::PreStep(){
   // virtual void AddEntity(CEntity & c_entity);
}

void CCrazyflieLoopFunctions::PostStep(){
   if (connect_established){
      if (GetSpace().GetSimulationClock() % TICKS_PER_SECOND == 0){
         transmitDroneData();
      }
   }
}

void CCrazyflieLoopFunctions::updateDroneCount(){
   unsigned sum = 0;
   CSpace::TMapPerType &m_cCrazyflies = GetSpace().GetEntitiesByType("crazyflie");
   for (CSpace::TMapPerType::iterator it = m_cCrazyflies.begin();
        it != m_cCrazyflies.end();
        ++it)
   {
      sum++;
   }
   droneCount = sum;
}

/****************************************/
/****************************************/

REGISTER_LOOP_FUNCTIONS(CCrazyflieLoopFunctions, "crazyflie_loop_functions");

// current_socket->on("message2", sio::socket::event_listener([&](sio::event &event)
//                                                            {
//                                                                // std::string eventMessage = event.get_message()->get_map()["data"]->get_string();
//                                                                // These are already imported... from the header file
//                                                                //! What ever is here is printed in the console because it is printed before QT opens (Stream not redirect)
//                                                                std::string eventMessage = event.get_message()->get_string();// .get()->;
//                                                                std::cout << green << "Received test message from Server. Message reads : " << eventMessage << std::endl << reset;
                                                               
//                                                                sio::message::ptr response = sio::string_message::create("Commande response");
//                                                                current_socket->emit("Agros Response event", response);

//                                                                // std::cout << cyan << eventMessage << reset << std::endl;
//                                                                //  std::cout << eventMessage << std::endl; }));

// current_socket->on("START", sio::socket::event_listener([&](sio::event &event)
//                                                         {
//       // pass info to controller to take off 
//       std::string eventMessage = event.get_message()->get_string(); // .get()->;
//       std::cout << "Command received from server" << eventMessage << std::endl;
//       std::cout << red << "Starting the mission" << std::endl; }));

// current_socket->on("FINISH", sio::socket::event_listener([&](sio::event &event)
//                                                          {
//                                                                // set state of controller to land
//                                                                std::string eventMessage = event.get_message()->get_string(); // .get()->;
//                                                                std::cout << "Command received from server" << eventMessage << std::endl;
//                                                                std::cout << red << "Stopping the mission" << std::endl << reset ; }));

// current_socket->on("command", sio::socket::event_listener([&](sio::event &event)
//                                                           {
//                                                              // std::string eventMessage = event.get_message()->get_map()["data"]->get_string();

//                                                              // sio::int_message::create();
//                                                              // sio::string_message response("Jawab");
//                                                              // sio::message::list li("jawab");
//                                                              // li.push(sio::string_message::create("economics"));

//                                                              // sio::message::ptr data = sio::object_message::create();
//                                                              std::string eventMessage = event.get_message()->get_string(); // .get()->;

//                                                              // sio::message::ptr data = sio::object_message::create();
//                                                              // sio::message::ptr data = sio
//                                                              // data->get_map().insert(std::pair<std::string, sio::message::ptr>("field1", "drones"));
//                                                              // data->get_map().insert(std::pair<std::string, sio::message::ptr>("field2", swarm));
//                                                              std::cout << "Command received from server " << eventMessage << std::endl;

//                                                              sio::message::ptr response = sio::string_message::create("Commande response");
//                                                              // sio::message reponse =  sio::string_message::create("Ceci est le message d'argos")

//                                                              // if (eventMessage == 1){
//                                                              //    current_socket->emit("Agros Response event", response);
//                                                              // }
//                                                           }));
