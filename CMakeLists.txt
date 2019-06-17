cmake_minimum_required(VERSION 2.8.4)
project(hideme)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES stdafx.cpp stdafx.h main.cpp Algorithm.cpp Algorithm.h CustomHeader.cpp CustomHeader.h Utils.cpp Utils.h)

add_executable(hideme ${SOURCE_FILES})
