cmake_minimum_required(VERSION 3.10)


project(ClassParser)


set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)


find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)


flex_target(Lexer lexer.l ${CMAKE_CURRENT_BINARY_DIR}/lex.yy.c)
bison_target(Parser parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.tab.c COMPILE_FLAGS "-d")


add_flex_bison_dependency(Lexer Parser)


add_executable(ClassParser ${BISON_Parser_OUTPUTS} ${FLEX_Lexer_OUTPUTS} parser.cpp)

# Линковка библиотек Flex и Bison
target_link_libraries(ClassParser ${FLEX_LIBRARIES})


target_include_directories(ClassParser PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
