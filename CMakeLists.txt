cmake_minimum_required (VERSION 2.8.8)

project (ycc1)

# Enable C++11 as support
set(CUSTOM_FLAGS "-Wall -std=c++11 -Wno-deprecated-register -Wno-unused-variable -g")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_DEBUG} ${CUSTOM_FLAGS}")

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

# flex and bison
find_package(FLEX REQUIRED)
flex_target(Lexer src/Lexer.l ${CMAKE_CURRENT_BINARY_DIR}/Lexer.yy.cc)
find_package(BISON REQUIRED)
bison_target(Parser src/Parser.y ${CMAKE_CURRENT_BINARY_DIR}/Parser.tab.cc)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

# Build ycc1
add_executable(ycc1 src/ycc1.cpp
  src/Driver.cpp src/AST.cpp src/LLVMCodeGen.cpp src/ContextManager.cpp src/Utils.cpp
  ${FLEX_Lexer_OUTPUTS} ${BISON_Parser_OUTPUTS})
target_link_libraries(ycc1 ${llvm_libs})

# Build test_parser
add_executable(test_parser test/test_parser.cpp
  src/Driver.cpp src/AST.cpp src/LLVMCodeGen.cpp src/ContextManager.cpp src/Utils.cpp
  ${FLEX_Lexer_OUTPUTS} ${BISON_Parser_OUTPUTS})
target_link_libraries(test_parser ${llvm_libs})

# Build test_dumpdot
add_executable(test_dumpdot test/test_dumpdot.cpp
  src/Driver.cpp src/AST.cpp src/ASTDump.cpp src/LLVMCodeGen.cpp
  src/ContextManager.cpp src/Utils.cpp
  ${FLEX_Lexer_OUTPUTS} ${BISON_Parser_OUTPUTS})
target_link_libraries(test_dumpdot ${llvm_libs})

# Install examples
file(COPY test/examples/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/examples/)

# Test utils
add_executable(test_utils src/Utils.cpp test/test_utils.cpp)
