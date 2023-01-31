/* Include the controller definition */
#include "crazyflie_sensing.h"
/* Function definitions for XML parsing */
#include <argos3/core/utility/configuration/argos_configuration.h>
/* 2D vector definition */
#include <argos3/core/utility/math/vector2.h>
/* Logging */
#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/core/utility/math/angles.h>
#include <string>
#include <iostream>

/****************************************/

#define TICKS_PER_SECOND 10
#define BATTERY_THRESHOLD 0.3f
#define ARENA_CEILING 2.0f

/****************************************/

CCrazyflieSensing::CCrazyflieSensing() : m_pcDistance(NULL),
                                         m_pcPropellers(NULL),
                                       //   m_pcRotorSpeed(NULL),
                                         m_pcRNG(NULL),
                                         m_pcRABA(NULL),
                                         m_pcRABS(NULL),
                                         m_pcPos(NULL),
                                         m_pcBattery(NULL),
                                         m_droneNumber(0),
                                         m_uiCurrentStep(1),
                                         m_droneState(WAITING_INSTRUCTIONS),
                                         arenaBoundaries(-3.5f, 3.5f),
                                         randomXCoordinate(0.0f),
                                         randomYCoordinate(0.0f) {}

/****************************************/
/****************************************/

void CCrazyflieSensing::Init(TConfigurationNode &t_node){
   try{
      /*
       * Initialize sensors/actuators
       */
      m_pcDistance = GetSensor<CCI_CrazyflieDistanceScannerSensor>("crazyflie_distance_scanner");
      m_pcPropellers = GetActuator<CCI_QuadRotorPositionActuator>("quadrotor_position");
      // m_pcRotorSpeed = GetActuator<CCI_QuadRotorSpeedActuator>("quadrotor_speed");

      /* Get pointers to devices */
      m_pcRABA = GetActuator<CCI_RangeAndBearingActuator>("range_and_bearing");
      m_pcRABS = GetSensor<CCI_RangeAndBearingSensor>("range_and_bearing");

      try{
         m_pcPos = GetSensor<CCI_PositioningSensor>("positioning");
      }catch (CARGoSException &ex){}

      try{
         m_pcBattery = GetSensor<CCI_BatterySensor>("battery");
      }catch (CARGoSException &ex){}
   }
   catch (CARGoSException &ex){
      THROW_ARGOSEXCEPTION_NESTED("Error initializing the crazyflie sensing controller for robot \"" << GetId() << "\"", ex);
   }

   std::cout << "Init DRONE"<<std::endl;

   m_droneNumber = stoi(GetId().substr(5));

   m_pcRNG = CRandom::CreateRNG("argos");
   
   m_cInitialPosition = m_pcPos->GetReading().Position;

   randomXCoordinate = m_pcRNG->Uniform(arenaBoundaries);
   randomYCoordinate = m_pcRNG->Uniform(arenaBoundaries);

   targetPosition = CVector3(randomXCoordinate, randomYCoordinate, ARENA_CEILING);

   velocityManager.updateDistanceTraveled(getPosition());

   Reset();
}

/****************************************/
/****************************************/

/*
    * This function is called once every time step.
    * The length of the time step is set in the XML file.
*/
void CCrazyflieSensing::ControlStep(){
   // Increase step counter
   m_uiCurrentStep++;

   velocityManager.updateDistanceTraveled(getPosition());

   switch (m_droneState){
      case WAITING_INSTRUCTIONS:
         break;

      case TAKEOFF:
         TakeOff();
         std::cout << "Taking off" << std::endl;
         break;

      case LAND:
         Land();
         break;

      case EXPLORATION:
         if (getBattery() > BATTERY_THRESHOLD){
            if(TakeOff() == false){ // Finished Taking off
               Explore();
            }
         }else{
            setDroneState(RETURN);
         }
         break;

      case RETURN:
         

         break;

      default:
         break;
   }


   // // Main loop
   // int nInitSteps = 10;
   // int nTotalSteps = 200;
   // if ((m_droneState == TAKEOFF))
   // { // m_uiCurrentStep < nInitSteps Laisse 10 timesteps au drone pour decoller 
   //    std::cout << "Taking off" << std::endl;
   //    TakeOff();
   //    m_cInitialPosition = m_pcPos->GetReading().Position;
   // }
   // else if ((m_droneState == EXPLORATION)){
   //    Square Pattern
   //    Par rapport a la position initiale (0,0,0)
   //    CVector3 trans(0.0f, 0.0f, 0.0f);
   //    if ((m_uiCurrentStep - nInitSteps) < nTotalSteps / 4)
   //    {
   //       trans.SetX(-3.0f);
   //    }
   //    else if ((m_uiCurrentStep - nInitSteps) < 2 * nTotalSteps / 4)
   //    {
   //       trans.SetY(-3.0f);
   //       trans.SetX(-3.0f);
   //    }
   //    else if ((m_uiCurrentStep - nInitSteps) < 3 * nTotalSteps / 4)
   //    {
   //       trans.SetY(-3.0f);
   //    }
   //    else
   //    {
   //       trans.SetY(0.0f);
   //       trans.SetX(0.0f);
   //    }
   //    CVector3 currentPosition = m_pcPos->GetReading().Position;
   //    CVector3 relativePositionCommand = (m_cInitialPosition + trans) - currentPosition;

   //    m_pcPropellers->SetRelativePosition(relativePositionCommand);
   // }
   // else if (m_droneState == LAND){
   //    Land();
   // }
   

   if ((m_uiCurrentStep % TICKS_PER_SECOND) == 0)
   {
      debugLog();
   }
}

/****************************************/
/****************************************/

bool CCrazyflieSensing::TakeOff(){
   CVector3 cPos = m_pcPos->GetReading().Position;
   // Tant que le drone n'est pas arrive au plafond de l'arene, return false
   // Soucis de precision ? on ne veut pas que le drone soit exactement a notre position
   // Car c'est un float, c'est impossible ? 
   // Ou bien a c'est juste pour ne pas se faire chier ? 
   // Des que le drone est a 99% de la hauteur souhaite,
   // Plus besoin de monter.
   if (Abs(cPos.GetZ() - ARENA_CEILING) < 0.01f)
      return false;
   cPos.SetZ(2.0f);
   m_pcPropellers->SetAbsolutePosition(cPos);
   // m_pcPropellers->SetAbsoluteYaw(ToRadians(CDegrees(50.0f)));
   // m_pcPropellers->SetRelativePosition(CVector3(0.0f, 0.0f, 2.0f));
   return true;
}

/****************************************/
/****************************************/

bool CCrazyflieSensing::Land(){
   CVector3 cPos = m_pcPos->GetReading().Position;
   if (Abs(cPos.GetZ()) < 0.01f)
      return false;
   cPos.SetZ(0.1f);
   m_pcPropellers->SetAbsolutePosition(cPos);
   return true;
}

/****************************************/
/****************************************/

bool CCrazyflieSensing::Explore()
{
   // Square Pattern
   // Par rapport a la position initiale (0,0,0)
   // CVector3 currentPosition = m_pcPos->GetReading().Position;

   // CVector3 trans(0.0f, 0.0f, 0.0f);
   // trans.SetX(-3.0f);

   // CVector3 relativePositionCommand = (m_cInitialPosition + trans) - currentPosition;

   // // trans.SetY(-3.0f);
   // // trans.SetX(-3.0f);

   // // trans.SetY(-3.0f);

   // // trans.SetY(0.0f);
   // // trans.SetX(0.0f);

   // m_pcPropellers->SetRelativePosition(relativePositionCommand);

   // m_pcPropellers->SetAbsoluteYaw(CRadians::PI_OVER_TWO);
}

/****************************************/
/****************************************/

void CCrazyflieSensing::Reset(){
   
   m_uiCurrentStep = 0 ;
   m_droneState = WAITING_INSTRUCTIONS;
}

/****************************************/
/****************************************/

MissionState CCrazyflieSensing::getDroneState(void){
   return this->m_droneState;
}

void CCrazyflieSensing::setDroneState(MissionState state){
   this->m_droneState = state;
}

unsigned CCrazyflieSensing::getCurrentDroneNumber(){
   return this->m_droneNumber;
}

/****************************************/
/****************************************/

argos::CVector3 CCrazyflieSensing::getPosition(){
   return this->m_pcPos->GetReading().Position;
}

/****************************************/
/****************************************/

argos::Real CCrazyflieSensing::getBattery(){
   return this->m_pcBattery->GetReading().AvailableCharge;
}

/****************************************/
/****************************************/

std::vector<argos::Real> CCrazyflieSensing::getDistanceScannerReading(){
   // Look here for documentation on the distance sensor: /root/argos3/src/plugins/robots/crazyflie/control_interface/ci_crazyflie_distance_scanner_sensor.h
   // Read and print distance sensor measurements
   std::vector<Real> readings;
   CCI_CrazyflieDistanceScannerSensor::TReadingsMap sDistRead =
       m_pcDistance->GetReadingsMap();

   auto iterDistRead = sDistRead.begin();

   if (sDistRead.size() == 4){
      readings.push_back((iterDistRead++)->second);
      readings.push_back((iterDistRead++)->second);
      readings.push_back((iterDistRead++)->second);
      readings.push_back((iterDistRead++)->second);
   }else{
      readings.push_back(-2);
      readings.push_back(-2);
      readings.push_back(-2);
      readings.push_back(-2);
   }
   return readings;
}

void CCrazyflieSensing::debugLog(){

   switch (m_droneState){
      case WAITING_INSTRUCTIONS:
         LOG << "Current state for drone n#" << m_droneNumber << " : Waiting for instructions" << std::endl;
         break;
      
      case TAKEOFF:
         LOG << "Current state for drone n#" << m_droneNumber << " : Taking off" << std::endl;
         break;
      
      case LAND:
         LOG << "Current state for drone n#" << m_droneNumber << " : Landing" << std::endl;
         break;
      
      case EXPLORATION:
         LOG << "Current state for drone n#" << m_droneNumber << " : Exploring" << std::endl;
         break;
      
      case RETURN:
         LOG << "Current state for drone n#" << m_droneNumber << " : Returning to base" << std::endl;
         break;

      default:
         break;
   }
   // Print current position.
   CVector3 position = getPosition();
   LOG << "Position (x,y,z) = ("
       << position.GetX() << ","
       << position.GetY() << ","
       << position.GetZ() << ")"
       << std::endl;
   // Print current battery level
   LOG << "Battery level: " << getBattery() << std::endl;

   std::vector<Real> readings = getDistanceScannerReading();
   LOG << "Front dist: " << readings[0] << std::endl;
   LOG << "Left dist: "  << readings[1] << std::endl;
   LOG << "Back dist: "  << readings[2] << std::endl;
   LOG << "Right dist: " << readings[3] << std::endl;

}

/****************************************/
/****************************************/

/*
 * This statement notifies ARGoS of the existence of the controller.
 * It binds the class passed as first argument to the string passed as
 * second argument.
 * The string is then usable in the XML configuration file to refer to
 * this controller.
 * When ARGoS reads that string in the XML file, it knows which controller
 * class to instantiate.
 * See also the XML configuration files for an example of how this is used.
 */
REGISTER_CONTROLLER(CCrazyflieSensing, "crazyflie_sensing_controller")
