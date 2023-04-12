include(FetchContent)

FetchContent_Declare(json
        URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz
        DOWNLOAD_EXTRACT_TIMESTAMP true)
FetchContent_MakeAvailable(json)

add_library(LLVM2COL STATIC
        Transform.cpp
        Instruction/UnaryOpTransform.cpp
        Instruction/BinaryOpTransform.cpp
        Instruction/MemoryOpTransform.cpp
        Instruction/CastOpTransform.cpp
        Instruction/FuncletPadOpTransform.cpp
        Instruction/OtherOpTransform.cpp
        Instruction/TermOpTransform.cpp
        BlockTransform.cpp
        Origin/OriginProvider.cpp
        Origin/ShortPositionDeriver.cpp
        Origin/ContextDeriver.cpp
        Origin/PreferredNameDeriver.cpp)
target_link_libraries(LLVM2COL proto-objects Util nlohmann_json::nlohmann_json)
target_include_directories(LLVM2COL PRIVATE
        "${CMAKE_SOURCE_DIR}/include"
        "${PROTO_BINARY_DIR}"
        )