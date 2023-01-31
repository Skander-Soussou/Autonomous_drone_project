/* Include the controller definition */
#include "VelocityManager.h"
/* Function definitions for XML parsing */
#include <argos3/core/utility/configuration/argos_configuration.h>
/* 2D vector definition */
#include <argos3/core/utility/math/vector3.h>
/* Logging */
#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/core/utility/math/angles.h>
#include <string>
#include <iostream>

/****************************************/

/****************************************/

VelocityManager::VelocityManager() : lastPosition(CVector3(0.0f, 0.0f, 0.0f)),
                                     currentPosition(CVector3(0.0f, 0.0f, 0.0f)) {
                                     }

/****************************************/
/****************************************/

CVector3 VelocityManager::getInstantVelocity(){
   CVector3 velocity;
   // ! TOO PRECISE ? 
   Real X = abs(getCurrentDistance().GetX()) / TIMESTEP;
   Real Y = abs(getCurrentDistance().GetY()) / TIMESTEP;
   Real Z = abs(getCurrentDistance().GetZ()) / TIMESTEP;
   return CVector3(X,Y,Z);
}

CVector3 VelocityManager::getCurrentDistance(){
   return currentPosition-lastPosition;
}

void VelocityManager::updateDistanceTraveled(CVector3 newPosition)
{
   lastPosition = currentPosition;
   currentPosition = newPosition;
}

void VelocityManager::goStraight(){}