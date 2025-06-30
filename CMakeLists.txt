cmake_minimum_required(VERSION 4.0)

project(todoer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(include)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(todoer ${SOURCES})
