add_library(Util STATIC
        Conversion/Conversion.cpp
        Exceptions.cpp
        Conversion/Instruction/UnaryOpConversion.cpp
        Conversion/Instruction/BinaryOpConversion.cpp
        Conversion/Instruction/MemoryOpConversion.cpp
        Conversion/Instruction/CastOpConversion.cpp
        Conversion/Instruction/FuncletPadOpConversion.cpp
        Conversion/Instruction/OtherOpConversion.cpp
        )
target_link_libraries(Util proto-objects)
target_include_directories(Util PRIVATE
        "${CMAKE_SOURCE_DIR}/include"
        "${PROTO_BINARY_DIR}"
        )
