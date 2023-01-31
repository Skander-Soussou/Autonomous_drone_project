# Sources :
# https://github.com/google/googletest/tree/master/googletest#incorporating-into-an-existing-cmake-project 
# https://crascit.com/2015/07/25/cmake-gtest/ 
# https://stackoverflow.com/questions/38296756/what-is-the-idiomatic-way-in-cmake-to-add-the-fpic-compiler-option

set (CMAKE_CXX_STANDARD 11)

add_library(socketIO SHARED IMPORTED)

set_property(TARGET socketIO PROPERTY IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/socket.io-client-cpp/libsioclient.a)

set(
  crazyflie_loop_functions_SOURCES
  crazyflie_loop_functions.h
  crazyflie_loop_functions.cpp
  )

add_library(crazyflie_loop_functions MODULE
  ${crazyflie_loop_functions_SOURCES})

# Adds libraries to link to this module when it's about to be built 
target_link_libraries(
  # The target module
  crazyflie_loop_functions
  # List of modules that will be linked and compiled first before this one 
  socketIO
  argos3core_simulator
  argos3plugin_simulator_entities
  argos3plugin_simulator_crazyflie
)
