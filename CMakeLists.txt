cmake_minimum_required(VERSION 3.5.0)
project(web-server VERSION 0.1.0 LANGUAGES C CXX)

add_executable(web-server main.cpp)

include(CTest)
enable_testing()

