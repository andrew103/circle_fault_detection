cmake_minimum_required(VERSION 3.8.0)
project(faultdetection-circle)

if(CMAKE_BUILD_TYPE STREQUAL "")
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel ..." FORCE)
endif(CMAKE_BUILD_TYPE STREQUAL "")

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find the ARGoS package, make sure to save the ARGoS prefix
find_package(ARGoS REQUIRED)
include_directories(${CMAKE_SOURCE_DIR} ${ARGOS_INCLUDE_DIRS})
link_directories(${ARGOS_LIBRARY_DIR})
link_libraries(${ARGOS_LDFLAGS})

# Check for Buzz
find_package(Buzz REQUIRED)
include_directories(${BUZZ_C_INCLUDE_DIR})


# Descend into the homswarm_loop_functions directory
add_subdirectory(loop_functions)
#add_subdirectory(proximity_modified)

#add_library(loop_functions SHARED)
#
#target_link_libraries(loop_functions 
#  project_loop_functions
#  proximity-lf)


# Descend into the foraging_loop_functions directory
# add_subdirectory(foraging_loop_functions)

# Descend into the homswarm_loop_functions directory
# add_subdirectory(homswarm_loop_functions)

# If Qt+OpenGL dependencies were found, descend into these directories
# if(ARGOS_COMPILE_QTOPENGL)
#  add_subdirectory(trajectory_loop_functions)
#  add_subdirectory(id_loop_functions)
# endif(ARGOS_COMPILE_QTOPENGL)

