add_library(AST STATIC
        Node.cpp)
target_link_libraries(AST proto-objects Util LLVM2COL)
target_include_directories(AST PRIVATE
        "${CMAKE_SOURCE_DIR}/include"
        "${PROTO_BINARY_DIR}"
        )