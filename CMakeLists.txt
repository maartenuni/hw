
cmake_minimum_required(VERSION 3.0)

project (mpc3280 C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic -std=gnu99")
find_library(wire name wiringPi)
find_library(realtime name rt)

add_executable(bitbang
                delay.c
                delay.h
                bitwriter.c
                bitwriter.h
                mcp3208.c
                mcp3208.h
                readmcp3208.c
                )

target_link_libraries(bitbang wiringPi rt)

