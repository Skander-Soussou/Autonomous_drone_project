add_library(crazyflie_sensing SHARED crazyflie_sensing.h crazyflie_sensing.cpp)
add_library(VelocityManager SHARED VelocityManager.h VelocityManager.cpp)

target_link_libraries(
  crazyflie_sensing
  VelocityManager
  argos3core_simulator
  argos3plugin_simulator_crazyflie
  argos3plugin_simulator_genericrobot
  )
