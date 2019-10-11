cmake_minimum_required(VERSION 3.14.3)
project(LLVM_Emulator)

set(CMAKE_CXX_STANDARD 14)
# set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(src/tool)
add_subdirectory(src/solver)
add_subdirectory(src/execution)
add_subdirectory(src/simplify)
