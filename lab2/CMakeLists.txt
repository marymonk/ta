cmake_minimum_required(VERSION 3.16)

project(lab2)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -g")

add_subdirectory(AST)
add_subdirectory(DFA)
add_subdirectory(myRegex)

add_executable(lab main.cpp)
target_link_libraries(lab myRegex)