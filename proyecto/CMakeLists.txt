cmake_minimum_required(VERSION 3.21)
project(proyecto)

set(CMAKE_CXX_STANDARD 14)

add_executable(proyecto main.cpp Analizador/parser.cpp Analizador/scanner.cpp)
