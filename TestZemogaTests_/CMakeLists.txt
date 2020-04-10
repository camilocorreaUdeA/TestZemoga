cmake_minimum_required(VERSION 3.12)

SET(CMAKE_CXX_COMPILER ${CXX})
project (TestZemogaTests)
include_directories(inc)
#Can manually add the sources using the set command as follows:
#set(SOURCES src/main.cpp src/custom.cpp)
#However, the file(GLOB...) allows for wildcard additions:
file(GLOB SOURCES "src/*.cpp")
add_executable(TestZemogaTests ${SOURCES})