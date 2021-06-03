cmake_minimum_required(VERSION 3.0)

project(pongx)

#Enable C++17 for MSVC
if (MSVC)
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_FLAGS "/permissive-") #Disable weird errors. I hate MSVC
#And C++17 for GCC
elseif(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_STANDARD 17)
else()
	message(SEND_ERROR "Unknown compiler")
endif()

#Find source files
file(GLOB_RECURSE pongx_SRC ${PROJECT_SOURCE_DIR}/src/*.cpp)
add_executable(pongx ${pongx_SRC})

#SFML
find_package(SFML COMPONENTS graphics REQUIRED)
target_link_libraries(pongx sfml-graphics)

install(TARGETS pongx RUNTIME DESTINATION bin)

#Copy resources
file(COPY res/ DESTINATION .)
