cmake_minimum_required (VERSION 2.8.8)

project (ycc1)

# Enable C++11 as support
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Add custom include directory
include_directories(include)

# Configurations for llvm
find_package(LLVM REQUIRED CONFIG)
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# Build Kaleidoscope compiler
add_executable(kaleidoscope src/kaleidoscope.cpp src/dumpdot.cpp)
llvm_map_components_to_libnames(llvm_libs core mcjit native)
target_link_libraries(kaleidoscope ${llvm_libs})

# Build test_parser
find_package(FLEX REQUIRED)
flex_target(Lexer src/Lexer.l ${CMAKE_CURRENT_BINARY_DIR}/Lexer.yy.cc)
find_package(BISON REQUIRED)
bison_target(Parser src/Parser.y ${CMAKE_CURRENT_BINARY_DIR}/Parser.tab.cc)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
add_executable(test_parser test/test_parser.cpp
  src/ParsingDriver.cpp src/AST.cpp
  ${FLEX_Lexer_OUTPUTS} ${BISON_Parser_OUTPUTS})

# Install examples
file(COPY test/examples/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/examples/)
