cmake_minimum_required(VERSION 3.11)
project(VcLLVM)

set(CMAKE_CXX_STANDARD 20)

# LLVM
find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})

link_directories(${LLVM_LIBRARY_DIRS})


# LLVM is normally built without RTTI. Be consistent with that.
if (NOT LLVM_ENABLE_RTTI)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
endif ()

llvm_map_components_to_libnames(LLVM_LIBS core support irreader passes analysis)
# end LLVM


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")

add_subdirectory(proto)
add_subdirectory(lib)
add_subdirectory(tools)
add_subdirectory(test)

install(TARGETS VCLLVM DESTINATION bin)
