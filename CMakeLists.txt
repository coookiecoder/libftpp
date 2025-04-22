cmake_minimum_required(VERSION 3.31)
project(libftpp)

set(CMAKE_CXX_STANDARD 20)

add_library(libftpp STATIC include/Pool.hpp include/PoolBuffer.hpp)
