add_library(VCLLVM_PASSES STATIC
        Function/BlockMapper.cpp
        Function/FunctionContractDeclarer.cpp
        Function/FunctionDeclarer.cpp
        Function/PureAssigner.cpp
        Function/FunctionInstructionTransformer.cpp
        )
target_link_libraries(VCLLVM_PASSES proto-objects Util)
target_include_directories(VCLLVM_PASSES PRIVATE
        "${CMAKE_SOURCE_DIR}/include"
        "${PROTO_BINARY_DIR}"
        )