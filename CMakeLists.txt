cmake_minimum_required(VERSION 3.3)
project(natural_language)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    cfg.cpp)

add_executable(natural_language ${SOURCE_FILES})