cmake_minimum_required(VERSION 2.8.4)
project(HideMeIn)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp main.h Algorithm.cpp Algorithm.h CustomHeader.cpp CustomHeader.h)
add_executable(HideMeIn ${SOURCE_FILES})