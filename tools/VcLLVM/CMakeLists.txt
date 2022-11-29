llvm_map_components_to_libnames(LLVM_LIBS_ core support irreader passes analysis)

set(SOURCE_FILES VcLLVM.cpp ../../include/Util/UtilIR.h)
add_executable(VcLLVM ${SOURCE_FILES})
target_include_directories(
        VcLLVM
        PRIVATE
        "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(VcLLVM ${LLVM_LIBS_} VcLLVM_PASSES AST Util)
