add_library(Util STATIC
        Exceptions.cpp
        )
target_link_libraries(Util proto-objects OriginProvider)
target_include_directories(Util PRIVATE
        "${CMAKE_SOURCE_DIR}/include"
        "${PROTO_BINARY_DIR}"
        )
