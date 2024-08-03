cmake_minimum_required(VERSION 3.16)

project(DFA)

set(CMAKE_CXX_STANDARD 20)

add_library(DFA STATIC source/DFA.cpp)
add_library(DFANode STATIC source/DFANode.cpp)

target_link_libraries(DFA DFANode)
target_link_libraries(DFA AST)

target_include_directories(DFA PUBLIC include)
target_include_directories(DFANode PUBLIC include)

