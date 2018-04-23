cmake_minimum_required(VERSION 3.0) 
project(MC)

set(CMAKE_BUILD_TYPE Release)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra -std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-Ofast") # optimize for speed 


#the management of all dependencies is delegated to conanfile.txt  
#conan BEGIN
include("${CMAKE_BINARY_DIR}/conanbuildinfo.cmake") 
conan_basic_setup() # this is for conan-specific configuration 
#conan END


#additional headers 
include_directories(
	"include"
	)
#source files
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.c")
 
add_executable(MC01 ${SOURCES})

