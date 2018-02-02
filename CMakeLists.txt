cmake_minimum_required(VERSION 3.0) 
project(MC)
 
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11" "-Ofast")
set(CMAKE_CXX "g++")


# modify to your own custom directory 
set(EIGEN_ROOT "/usr/local/include/eigen3")

#all headers
include_directories(
	"include"
	${EIGEN_ROOT}
	)

#custom link directories
link_directories(
	"lib"
	)
#source files
file(GLOB_RECURSE SOURCES "src/*.cpp")
 
add_executable(MC01 ${SOURCES})

#target_link_libraries(MC01 "libname")

