cmake_minimum_required(VERSION 3.0)

project(pongx)

######################
# Compiler options
######################
#Enable C++17 for MSVC
if (MSVC)
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_FLAGS "/permissive-") #Disable weird errors. I hate MSVC
#And C++17 for GCC
elseif(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_STANDARD 17)
	#Enable pthread for GCC
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
else()
	message(SEND_ERROR "Unknown compiler")
endif()

######################
# Add subdirectories
######################
add_subdirectory(src)
add_subdirectory(tst)
