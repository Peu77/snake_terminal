cmake_minimum_required(VERSION 3.27)
project(snake_server C)

set(CMAKE_C_STANDARD 23)

add_executable(snake_server server.c)