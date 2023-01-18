project(framework)
cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O3 -fno-omit-frame-pointer -fconcepts")

set(CMAKE_CXX_FLAGS_ASAN "-g -fsanitize=address,undefined -fno-sanitize-recover=all -DSAN"
    CACHE STRING "Compiler flags in asan build"
    FORCE)

include_directories(include)

add_executable(solution1 src/solution1.cpp)
add_executable(solution2 src/solution2.cpp)