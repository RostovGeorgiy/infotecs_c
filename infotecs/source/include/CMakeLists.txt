project(library)
cmake_minimum_required(VERSION 3.00)
set(CMAKE_CXX_STANDARD 11)

add_library(${PROJECT_NAME} SHARED funcslib.cpp funcs.h)
target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
