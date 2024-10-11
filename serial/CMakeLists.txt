cmake_minimum_required(VERSION 3.22)
project(serial C)

set(CMAKE_C_STANDARD 23)

add_executable(serial main.c)
target_link_libraries(serial m)

