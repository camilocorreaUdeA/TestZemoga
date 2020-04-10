cmake_minimum_required(VERSION 3.12)
project(TestZemogaConsoleApp VERSION 1.0.0)

add_executable(TestZemogaConsoleApp app.cpp) # Create an executable for the app

target_link_libraries(TestZemogaConsoleApp PUBLIC TestZemogaBmpLib) # Ask the linker to link executable and library
