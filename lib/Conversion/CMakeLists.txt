add_library(LLVM2COL STATIC
        Conversion.cpp
        Instruction/UnaryOpConversion.cpp
        Instruction/BinaryOpConversion.cpp
        Instruction/MemoryOpConversion.cpp
        Instruction/CastOpConversion.cpp
        Instruction/FuncletPadOpConversion.cpp
        Instruction/OtherOpConversion.cpp
        )
target_link_libraries(LLVM2COL proto-objects Util)
target_include_directories(LLVM2COL PRIVATE
        "${CMAKE_SOURCE_DIR}/include"
        "${PROTO_BINARY_DIR}"
        )