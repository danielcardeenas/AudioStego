cmake_minimum_required(VERSION 2.8.4)
project(hideme)

FIND_PACKAGE( Boost 1.40 COMPONENTS program_options REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp main.h Algorithm.cpp Algorithm.h CustomHeader.cpp CustomHeader.h)
add_executable(hideme ${SOURCE_FILES})
TARGET_LINK_LIBRARIES( hideme ${Boost_LIBRARIES} )
