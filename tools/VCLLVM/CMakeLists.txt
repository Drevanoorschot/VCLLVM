set(SOURCE_FILES VCLLVM.cpp)
add_executable(VCLLVM ${SOURCE_FILES})
target_include_directories(
        VCLLVM
        PRIVATE
        "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(VCLLVM ${LLVM_LIBS} VCLLVM_PASSES proto-objects)
