cmake_minimum_required(VERSION 2.8.4)
project(periodlib)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES src/main.cpp src/dbf.cpp src/etc.cpp)
add_executable(periodlib ${SOURCE_FILES})