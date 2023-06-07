cmake_minimum_required(VERSION 3.22)
project(rccar)

set(CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_FLAGS "-O3")
find_package(Threads)

add_subdirectory(code)
