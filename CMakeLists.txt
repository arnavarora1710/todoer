cmake_minimum_required(VERSION 3.14)

project(todoer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Debug flags for development and debugging
set(DEBUG_FLAGS -g -O0 -Wall -Wextra -Wpedantic -fsanitize=address -fsanitize=undefined)

# Release flags for performance testing and benchmarks
set(RELEASE_FLAGS -O3 -DNDEBUG -flto -Wall -Wextra -Wpedantic)

include_directories(include)

file(GLOB_RECURSE ALL_SRC CONFIGURE_DEPENDS "src/*.cpp")

set(MAIN_SRC "src/Todoer.cpp")
list(FILTER ALL_SRC EXCLUDE REGEX ".*Todoer\\.cpp$")
set(CORE_SRC ${ALL_SRC})  # this is what test_runner will use

# App executable (DEBUG MODE for development)
add_executable(${PROJECT_NAME} ${CORE_SRC} ${MAIN_SRC})
target_compile_options(${PROJECT_NAME} PRIVATE ${DEBUG_FLAGS})
target_link_options(${PROJECT_NAME} PRIVATE ${DEBUG_FLAGS})

# Test executable (RELEASE MODE for performance benchmarks)
file(GLOB_RECURSE TEST_SRC "tests/*.cpp")
add_executable(test_runner ${CORE_SRC} ${TEST_SRC})
target_compile_options(test_runner PRIVATE ${RELEASE_FLAGS})
target_link_options(test_runner PRIVATE ${RELEASE_FLAGS})

enable_testing()
add_test(NAME TestLexer COMMAND test_runner)
add_test(NAME TestInterpreter COMMAND test_runner)
