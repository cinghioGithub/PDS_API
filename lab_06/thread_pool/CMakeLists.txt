cmake_minimum_required(VERSION 3.17)
project(thread_pool)

set(CMAKE_CXX_STANDARD 20)

SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread")

add_executable(thread_pool main.cpp)