cmake_minimum_required(VERSION 4.1)
project(tinyC)

set(CMAKE_CXX_STANDARD 20)

add_executable(tinyC main.cc
        lexer.cc
)
