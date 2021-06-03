cmake_minimum_required(VERSION 3.0)

project(pongx)

add_executable(pongx main.cpp)

install(TARGETS pongx RUNTIME DESTINATION bin)
