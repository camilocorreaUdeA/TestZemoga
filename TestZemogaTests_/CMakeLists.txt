cmake_minimum_required(VERSION 2.8)

SET(CMAKE_CXX_COMPILER ${CXX})
project (TestZemogaTests)
include_directories(inc)
file(GLOB SOURCES "src/*.cpp")
add_executable(TestZemogaTests ${SOURCES})
