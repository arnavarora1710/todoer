cmake_minimum_required(VERSION 3.14)

project(todoer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(include)

file(GLOB_RECURSE ALL_SRC "src/*.cpp")

set(MAIN_SRC "src/main.cpp")
list(FILTER ALL_SRC EXCLUDE REGEX ".*main\\.cpp$")
set(CORE_SRC ${ALL_SRC})  # this is what test_runner will use

# App executable
add_executable(todoer ${CORE_SRC} ${MAIN_SRC})

# Test executable
file(GLOB_RECURSE TEST_SRC "tests/*.cpp")
add_executable(test_runner ${CORE_SRC} ${TEST_SRC})

enable_testing()
add_test(NAME test_parser COMMAND test_runner)
