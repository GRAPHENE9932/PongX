cmake_minimum_required(VERSION 3.0)

project(pongx)

add_executable(pongx main.cpp)

#SFML
find_package(SFML COMPONENTS graphics REQUIRED)
target_link_libraries(pongx sfml-graphics)

install(TARGETS pongx RUNTIME DESTINATION bin)
