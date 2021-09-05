#cmake minimum version
cmake_minimum_required(VERSION 2.8)

#project name
project(ke-bear-microservice C CXX)

if(UNIX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fPIC -lrt -lpthread -g -ggdb")
    SET(DEPENDENT_LIBRARIES glog pthread)
endif()

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set (LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

#add subdirectory
add_subdirectory(src)
add_subdirectory (examples)
add_subdirectory (utest)
