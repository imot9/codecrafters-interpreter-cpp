cmake_minimum_required(VERSION 3.13)

project(codecrafters-interpreter)

set(CMAKE_CXX_STANDARD 23)

file(GLOB_RECURSE SOURCE_FILES src/*.cpp src/*.hpp)

add_executable(interpreter ${SOURCE_FILES})