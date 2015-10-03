cmake_minimum_required (VERSION 2.8.8)

project (ycc1)

# Enable C++11 as support
add_definitions(-std=c++11)

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
