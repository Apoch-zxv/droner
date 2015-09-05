cmake_minimum_required(VERSION 3.0)
project(droner)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    fry/blink.cpp
    fry/CMakeLists.txt
    README.md)

add_executable(droner ${SOURCE_FILES})