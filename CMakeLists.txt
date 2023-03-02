cmake_minimum_required(VERSION 3.24)
project(calender_app C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(calender_app
        calender.c)
