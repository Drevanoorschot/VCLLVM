add_library(VcLLVM_PASSES STATIC
        FunctionDeclarer.cpp
        )
target_link_libraries(VcLLVM_PASSES proto-objects)
target_include_directories(VcLLVM_PASSES PRIVATE
        "${CMAKE_SOURCE_DIR}/include"
        "${PROTO_BINARY_DIR}"
        )