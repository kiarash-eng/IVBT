cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 ")

project(TG60)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++") 


include(FindPkgConfig)

find_package(Geant4 REQUIRED ui_all vis_all)
include(${Geant4_USE_FILE})

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(TG60 TG60.cc ${sources} ${headers})
target_link_libraries(TG60 ${Geant4_LIBRARIES}) 

install(TARGETS TG60 DESTINATION bin)

set(CPACK_GENERATOR "TGZ")
include(CPack)

