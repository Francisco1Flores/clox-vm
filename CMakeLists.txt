cmake_minimum_required(VERSION 3.10)

project(clox)

add_executable(clox main.c chunk.c debug.c memory.c value.c)
