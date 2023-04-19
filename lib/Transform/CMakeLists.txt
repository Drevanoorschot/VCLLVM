

add_library(LLVM2COL STATIC
        Transform.cpp
        Instruction/UnaryOpTransform.cpp
        Instruction/BinaryOpTransform.cpp
        Instruction/MemoryOpTransform.cpp
        Instruction/CastOpTransform.cpp
        Instruction/FuncletPadOpTransform.cpp
        Instruction/OtherOpTransform.cpp
        Instruction/TermOpTransform.cpp
        BlockTransform.cpp)
target_link_libraries(LLVM2COL proto-objects Util OriginProvider)
target_include_directories(LLVM2COL PRIVATE
        "${CMAKE_SOURCE_DIR}/include"
        "${PROTO_BINARY_DIR}"
        )