cmake_minimum_required(VERSION 3.22)
project(parallel_read_write_lock C)

set(CMAKE_C_STANDARD 23)

add_executable(parallel_read_write_lock main.c)
target_link_libraries(parallel_read_write_lock m)
