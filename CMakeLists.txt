# Especifica la versión mínima de CMake
cmake_minimum_required(VERSION 3.30)

# Nombre del proyecto
project(clox C)

# Agrega el ejecutable
add_executable(clox main.c memory.c chunk.c value.c debug.c vm.c compiler.c scanner.c object.c)
