add_library(project_loop_functions SHARED 
  project.h
  project.cpp)

target_link_libraries(project_loop_functions
  ${BUZZ_LIBRARY}
  argos3core_simulator
  argos3plugin_simulator_dynamics2d
  argos3plugin_simulator_entities
  argos3plugin_simulator_kheperaiv
  argos3plugin_simulator_genericrobot
  argos3plugin_simulator_media
  argos3plugin_simulator_qtopengl
  argos3plugin_simulator_buzz)
