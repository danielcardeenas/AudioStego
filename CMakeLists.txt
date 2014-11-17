cmake_minimum_required(VERSION 2.8.4)
project(BuildRelease/HideMeIn)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp main.h Algorithm.cpp Algorithm.h CustomHeader.cpp CustomHeader.h)
add_executable(BuildRelease/HideMeIn ${SOURCE_FILES})