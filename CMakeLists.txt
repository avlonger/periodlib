cmake_minimum_required(VERSION 2.8.4)
project(periodlib)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(EXPERIMENT src/experiment.cpp src/dbf.cpp src/etc.cpp src/period.cpp src/kmp.cpp)
set(MEASURE src/measure.cpp src/dbf.cpp src/etc.cpp src/period.cpp src/kmp.cpp)
add_executable(experiment ${EXPERIMENT})
add_executable(measure ${MEASURE})
