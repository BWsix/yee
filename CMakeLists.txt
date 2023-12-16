cmake_minimum_required(VERSION 3.12)
project(yee C)
set(CMAKE_C_STANDARD 11)

add_executable(yee yee.c)

target_link_libraries(yee m)
