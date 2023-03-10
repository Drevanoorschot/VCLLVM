add_library(VCLLVM_PASSES STATIC
        Function/FunctionContractDeclarer.cpp
        Function/FunctionDeclarer.cpp
        Function/PureAssigner.cpp)
target_link_libraries(VCLLVM_PASSES proto-objects Util)
target_include_directories(VCLLVM_PASSES PRIVATE
        "${CMAKE_SOURCE_DIR}/include"
        "${PROTO_BINARY_DIR}"
        )