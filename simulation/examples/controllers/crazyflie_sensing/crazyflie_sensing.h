#ifndef CRAZYFLIE_SENSING_H
#define CRAZYFLIE_SENSING_H

/*
 * Include some necessary headers.
 */
/* Definition of the CCI_Controller class. */
#include <argos3/core/control_interface/ci_controller.h>
/* Definition of the crazyflie distance sensor */
#include <argos3/plugins/robots/crazyflie/control_interface/ci_crazyflie_distance_scanner_sensor.h>
/* Definition of the crazyflie position actuator */
#include <argos3/plugins/robots/generic/control_interface/ci_quadrotor_position_actuator.h>
/* Definition of the crazyflie position sensor */
#include <argos3/plugins/robots/generic/control_interface/ci_positioning_sensor.h>
/* Definition of the crazyflie range and bearing actuator */
#include <argos3/plugins/robots/generic/control_interface/ci_range_and_bearing_actuator.h>
/* Definition of the crazyflie range and bearing sensor */
#include <argos3/plugins/robots/generic/control_interface/ci_range_and_bearing_sensor.h>
/* Definition of the crazyflie battery sensor */
#include <argos3/plugins/robots/generic/control_interface/ci_battery_sensor.h>
/* Definitions for random number generation */
#include <argos3/core/utility/math/rng.h>
#include <iostream>
#include <fstream>
#include <vector>
#include "VelocityManager.h"


/*
 * All the ARGoS stuff in the 'argos' namespace.
 * With this statement, you save typing argos:: every time.
 */
using namespace argos;


enum MissionState
{
   NONE = 0, // internal state
   WAITING_INSTRUCTIONS,  
   TAKEOFF,
   LAND,
   EXPLORATION, // internal state
   RETURN
};

/*
 * A controller is simply an implementation of the CCI_Controller class.
 */
class CCrazyflieSensing : public CCI_Controller{

public:
   /* Class constructor. */
   CCrazyflieSensing();
   /* Class destructor. */
   virtual ~CCrazyflieSensing() {}

   /*
    * This function initializes the controller.
    * The 't_node' variable points to the <parameters> section in the XML
    * file in the <controllers><footbot_foraging_controller> section.
    */
   virtual void Init(TConfigurationNode &t_node);

   /*
    * This function is called once every time step.
    * The length of the time step is set in the XML file.
    */
   virtual void ControlStep();

   /*
    * This function resets the controller to its state right after the
    * Init().
    * It is called when you press the reset button in the GUI.
    */
   virtual void Reset();

   /*
    * Called to cleanup what done by Init() when the experiment finishes.
    * In this example controller there is no need for clean anything up,
    * so the function could have been omitted. It's here just for
    * completeness.
    */
   virtual void Destroy() {}

   /*
    * This function lifts the drone from the ground
    */
   bool TakeOff();

   /*
    * This function returns the drone to the ground
    */
   bool Land();
   
   /*
    * This function explores the environment
    */
   bool Explore();

   /*
    * Gets current drone state  :
    */
   MissionState getDroneState(void);

   /*
    * Changes internal drone state to value :
    */
   void setDroneState(MissionState state);

   /* 
    * In crazyflie_sensing.argos, entity names are stored as drone_N, N being 
    * the number of the drone, starting from 0. Here we retrive the N, and use
    * it as (an offset for) port number
    */
   unsigned getCurrentDroneNumber();

   CVector3 getPosition();

   Real getBattery();

   std::vector<argos::Real> getDistanceScannerReading();

   void debugLog();

private:

   VelocityManager velocityManager;
   /* Pointer to the crazyflie distance sensor */
   CCI_CrazyflieDistanceScannerSensor *m_pcDistance;

   /* Pointer to the position actuator */
   CCI_QuadRotorPositionActuator *m_pcPropellers;

   // CCI_QuadRotorSpeedActuator *m_pcRotorSpeed;

   /* Pointer to the range and bearing actuator */
   CCI_RangeAndBearingActuator *m_pcRABA;

   /* Pointer to the range and bearing sensor */
   CCI_RangeAndBearingSensor *m_pcRABS;

   /* Pointer to the positioning sensor */
   CCI_PositioningSensor *m_pcPos;

   /* Pointer to the battery sensor */
   CCI_BatterySensor *m_pcBattery;

   /* The random number generator */
   CRandom::CRNG *m_pcRNG;

   /* Current step */
   uint m_uiCurrentStep;

   /* Initial Position */
   CVector3 m_cInitialPosition;

   uint8_t m_droneNumber;

   MissionState m_droneState;

   CRange<Real> arenaBoundaries;

   Real randomXCoordinate;
   Real randomYCoordinate;

   CVector3 targetPosition; 


   /* Maximum tolerance for the angle between
    * the robot heading direction and
    * the closest obstacle detected. */
   CDegrees m_cAlpha;

   /* Maximum tolerance for the proximity reading between
    * the robot and the closest obstacle.
    * The proximity reading is 0 when nothing is detected
    * and grows exponentially to 1 when the obstacle is
    * touching the robot.
    */
   Real m_fDelta;

   /* Angle tolerance range to go straight.
    * It is set to [-alpha,alpha]. */
   CRange<CRadians> m_cGoStraightAngleRange;
};

#endif
