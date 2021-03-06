add_executable(sym smain.cpp)

# LLVM
set(LLVM_INSTALL_DIR "${PROJECT_SOURCE_DIR}/../llvm" CACHE STRING "Path to LLVM folder")
list(APPEND CMAKE_MODULE_PATH ${LLVM_INSTALL_DIR}/lib/cmake/llvm)
include(LLVMConfig)

target_include_directories(sym PUBLIC ${LLVM_INCLUDE_DIRS})
target_link_libraries(sym PUBLIC LLVMBitReader LLVMBitWriter LLVMIRReader)

# Z3
set(Z3_INSTALL_DIR "${PROJECT_SOURCE_DIR}/../z3/build" CACHE STRING "Path to LLVM folder")
list(APPEND CMAKE_MODULE_PATH ${Z3_INSTALL_DIR})
include(Z3Config)

target_include_directories(sym PUBLIC ${Z3_CXX_INCLUDE_DIRS})
target_link_libraries(sym PUBLIC ${Z3_LIBRARIES})

# set relative include path
include_directories(..)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti -fno-stack-protector")
