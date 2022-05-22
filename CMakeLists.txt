cmake_minimum_required(VERSION 3.21)
project(2semextra)

set(CMAKE_CXX_STANDARD 20)

add_executable(2semextra main.cpp bin/matrix.cpp bin/matrix.h rationalnum/RationalNum.cpp rationalnum/RationalNum.h)
