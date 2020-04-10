cmake_minimum_required(VERSION 3.12)
project(TestZemogaAppProj VERSION 1.0.0)

set(CMAKE_CXX_COMPILER ${CXX})

set(CMAKE_CXX_STANDARD ${CXX_STAND})

set( CMAKE_CXX_FLAGS "-Wall -Werror -O3 " )

include_directories("bmp/inc") # We need this to resolve include files path

add_subdirectory(bmp) # Includes CMakeLists.txt from bmp folder (libs before executables)
add_subdirectory(app) # Includes CMakeLists.txt from app folder (executables after libs)