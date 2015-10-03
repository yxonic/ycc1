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

# Build test_flex_lexer
find_package(FLEX REQUIRED)
flex_target(Scanner src/scanner.l ${CMAKE_CURRENT_BINARY_DIR}/scanner.cpp)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
add_executable(test_flex_lexer test/test_flex_lexer.cpp ${FLEX_Scanner_OUTPUTS})
target_link_libraries(test_flex_lexer fl)

# Install examples
file(COPY test/examples/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/examples/)
