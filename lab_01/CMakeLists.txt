cmake_minimum_required(VERSION 3.17)
project(lab_01)

set(CMAKE_CXX_STANDARD 17)

add_executable(lab_01 main.cpp Message.cpp Message.h DurationLogger.cpp DurationLogger.h MessageStore.cpp MessageStore.h)