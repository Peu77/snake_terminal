cmake_minimum_required(VERSION 3.27)
project(snake_console C)

set(CMAKE_C_STANDARD 23)

add_subdirectory(server)

file(GLOB_RECURSE SOURCES src/*.c)
file(GLOB_RECURSE HEADERS include/*.h)

add_executable(snake_console ${SOURCES} ${HEADERS})