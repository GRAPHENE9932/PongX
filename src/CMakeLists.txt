cmake_minimum_required(VERSION 3.0)

project(pongx)

######################
# Libraries and sources
######################
#Find source files
file(GLOB_RECURSE pongx_SRC ${PROJECT_SOURCE_DIR}/*.cpp)
#add_executable(pongx_run ${pongx_SRC})
add_library(pongx_lib STATIC ${pongx_SRC})

#SFML
find_package(SFML COMPONENTS graphics REQUIRED)
target_link_libraries(pongx_lib sfml-graphics)

######################
# Executable
######################
add_executable(pongx_run ${PROJECT_SOURCE_DIR}/main.cpp)
target_link_libraries(pongx_run pongx_lib)

#Copy resources
file(COPY ../res/ DESTINATION .)
