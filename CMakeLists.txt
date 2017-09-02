cmake_minimum_required(VERSION 2.8) 
project(MC)
 
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "--std=c++11")
set(CMAKE_CXX "g++")

#Bring the headers, such as Student.h into the project
include_directories(
	include
	lib/eigen)

#Can manually add the sources using the set command as follows:
#set(SOURCES src/mainapp.cpp src/Student.cpp)
 
#However, the file(GLOB...) allows for wildcard additions:
file(GLOB SOURCES "src/*.cpp")
 
add_executable(MC01 ${SOURCES})


