llvm_map_components_to_libnames(LLVM_LIBS_ core support irreader passes)

set(SOURCE_FILES
        VcLLVM.cpp
        ../../lib/Passes/FunctionListToErr.cpp
        ../../lib/Passes/InstructionMapper.cpp)
add_executable(VcLLVM ${SOURCE_FILES})
target_include_directories(
        VcLLVM
        PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/../../include")
target_link_libraries(VcLLVM ${LLVM_LIBS_})
