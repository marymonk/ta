cmake_minimum_required(VERSION 3.16)

project(AST)

set(CMAKE_CXX_STANDARD 20)

add_library(AST STATIC source/AST.cpp)
add_library(ASTNodes STATIC source/Operations.cpp)
add_library(StackNodes STATIC source/StackNode.cpp)
add_library(CharNode STATIC source/CharNode.cpp)

target_link_libraries(AST ASTNodes StackNodes CharNode)

target_include_directories(AST PUBLIC include)
target_include_directories(ASTNodes PUBLIC include)
target_include_directories(StackNodes PUBLIC include)
target_include_directories(CharNode PUBLIC include)
