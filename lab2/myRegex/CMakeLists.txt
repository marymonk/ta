cmake_minimum_required(VERSION 3.16)

project(myRegex)

set(CMAKE_CXX_STANDARD 20)

add_library(myRegex STATIC source/myRegex.cpp)

target_link_libraries(myRegex DFA)

target_include_directories(myRegex PUBLIC include)

