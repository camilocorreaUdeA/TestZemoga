cmake_minimum_required(VERSION 3.12)
project(TestZemogaBmpLib VERSION 1.0.0)

file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp) # Library source files
file(GLOB HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/inc/*.h) # Library header files

message(STATUS "Current lib source dir: ${CMAKE_CURRENT_SOURCE_DIR}")

add_library(TestZemogaBmpLib
            ${SOURCES}
            ${HEADER_FILES}
            ) # Create the library (static / shared)
