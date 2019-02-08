cmake_minimum_required(VERSION 3.5)
project(ArgonCube2x2Geo)

add_definitions(-DVERSION="v04_00_00")
set(CMAKE_CXX_STANDARD 11)

# Geant4 dependencies
list(APPEND CMAKE_MODULE_PATH $ENV{GEANTDIR})
find_package(Geant4 REQUIRED gdml)
include_directories(${Geant4_INCLUDE_DIRS})
add_definitions(${Geant4_DEFINITIONS})

#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")

# RapidJSON dependencies
set(RapidJSON_DIR $ENV{RapidJSON_CMAKEDIR})
find_package(RapidJSON REQUIRED CONFIG)
include_directories(${RapidJSON_INCLUDE_DIRS})

# Project includes
include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB_RECURSE sources ${PROJECT_SOURCE_DIR}/src/*.cxx)
file(GLOB_RECURSE headers ${PROJECT_SOURCE_DIR}/include/*.h)
add_executable(construct ${PROJECT_SOURCE_DIR}/construct.cxx ${sources} ${headers})

target_link_libraries(construct ${Geant4_LIBRARIES})
