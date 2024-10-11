cmake_minimum_required(VERSION 3.22)
project(parallel_mutex C)

set(CMAKE_C_STANDARD 23)

add_executable(parallel_mutex main.c)
target_link_libraries(parallel_mutex m)
