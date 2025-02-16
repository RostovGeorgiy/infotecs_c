project(program1)
cmake_minimum_required(VERSION 3.00)
set(CMAKE_CXX_STANDARD 11)

add_executable(${PROJECT_NAME} program1.cpp)
target_link_libraries(${PROJECT_NAME} library)
