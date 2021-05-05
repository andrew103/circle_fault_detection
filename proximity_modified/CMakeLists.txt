cmake_minimum_required(VERSION 3.8.0)
project(proximity_modified)

# Compile code
add_library(proximity-lf SHARED
  proximity-lf.h proximity-lf.cpp
  kheperaiv_proximity_group6_sensor.h kheperaiv_proximity_group6_sensor.cpp)
target_link_libraries(proximity-lf
  ${BUZZ_LIBRARY}
  argos3core_simulator
  argos3plugin_simulator_dynamics2d
  argos3plugin_simulator_entities
  argos3plugin_simulator_kheperaiv
  argos3plugin_simulator_genericrobot
  argos3plugin_simulator_media
  argos3plugin_simulator_qtopengl
  argos3plugin_simulator_buzz)
