cmake_minimum_required(VERSION 2.8)
project(TestZemogaConsoleApp)

add_executable(TestZemogaConsoleApp app.cpp) # Create an executable for the app

target_link_libraries(TestZemogaConsoleApp PUBLIC TestZemogaBmpLib) # Ask the linker to link executable and library
