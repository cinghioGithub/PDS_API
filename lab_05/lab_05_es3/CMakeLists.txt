cmake_minimum_required(VERSION 3.19)
project(lab_05_es3)

set(CMAKE_CXX_STANDARD 20)

SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread")

add_executable(lab_05_es3 main.cpp)