add_library(VcLLVM_PASSES SHARED
        FunctionDeclarer.cpp
        )
target_include_directories(VcLLVM_PASSES PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/../../include"
        "$<BUILD_INTERFACE:${PROTO_BINARY_DIR}>"
        )