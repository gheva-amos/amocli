cmake_minimum_required(VERSION 3.20)
set(PROJECT_NAME argparse)
project(${PROJECT_NAME} LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(argparse
    flag.cpp
    pole.cpp
    subcommand.cpp
    argparse.cpp
    )
target_compile_options(argparse PRIVATE -Wall -Wextra -pedantic -Wno-enum-compare -Wno-maybe-uninitialized -Wno-uninitialized -Wno-unused-parameter)

