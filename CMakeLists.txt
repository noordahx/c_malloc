cmake_minimum_required(VERSION 3.0)
project(c_malloc C)

set(CMAKE_C_STANDARD 11)

add_executable(c_malloc main.c
        malloc.c
        malloc.h)
