cmake_minimum_required(VERSION 3.5)
project(Majorana)

add_definitions(-DVERSION="v01_05_00")

option(WITH_GEANT4_GDML "Build with Geant4 GDML parser." OFF)
option(WITH_GEANT4_UIVIS "Build with Geant4 UI and Vis drivers" ON)

set(CMAKE_CXX_STANDARD 11)

# Geant4 dependencies
list(APPEND CMAKE_MODULE_PATH $ENV{GEANTDIR})
if (WITH_GEANT4_GDML)
  find_package(Geant4 REQUIRED gdml)
else()
  find_package(Geant4 REQUIRED)
endif()
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()
include_directories(${Geant4_INCLUDE_DIRS})
add_definitions(${Geant4_DEFINITIONS})

if (WITH_GEANT4_GDML)
  add_definitions(-DG4_GDML)
endif()

# ROOT dependencies 
list(APPEND CMAKE_MODULE_PATH $ENV{ROOTSYS}/etc/cmake)
find_package(ROOT COMPONENTS REQUIRED)
include_directories(${ROOT_INCLUDE_DIRS})
add_definitions(${ROOT_CXX_FLAGS})

# RapidJSON dependencies
set(RapidJSON_DIR $ENV{RapidJSON_CMAKEDIR})
find_package(RapidJSON REQUIRED CONFIG)
include_directories(${RapidJSON_INCLUDE_DIRS})

# Project includes
include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB_RECURSE sources ${PROJECT_SOURCE_DIR}/src/*.cxx)
file(GLOB_RECURSE headers ${PROJECT_SOURCE_DIR}/include/*.h)
add_executable(simulate ${PROJECT_SOURCE_DIR}/bin/simulate.cxx ${sources} ${headers})

target_link_libraries(simulate ${ROOT_LIBRARIES})
target_link_libraries(simulate ${Geant4_LIBRARIES})
