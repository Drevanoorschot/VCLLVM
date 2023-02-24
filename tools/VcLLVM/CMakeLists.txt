llvm_map_components_to_libnames(LLVM_LIBS_ core support irreader passes analysis)

set(SOURCE_FILES VcLLVM.cpp)
add_executable(VCLLVM ${SOURCE_FILES})
target_include_directories(
        VCLLVM
        PRIVATE
        "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(VCLLVM ${LLVM_LIBS_} VcLLVM_PASSES proto-objects Util)
