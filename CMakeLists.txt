cmake_minimum_required(VERSION 2.8.4)
project(HideMeIn)

FIND_PACKAGE( Boost 1.60 COMPONENTS program_options REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp main.h Algorithm.cpp Algorithm.h CustomHeader.cpp CustomHeader.h)
add_executable(HideMeIn ${SOURCE_FILES})
TARGET_LINK_LIBRARIES( HideMeIn ${Boost_LIBRARIES} )
