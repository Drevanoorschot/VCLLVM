add_library(Util SHARED
        UtilIR.cpp)
target_include_directories(Util PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/../../include"
        "$<BUILD_INTERFACE:${PROTO_BINARY_DIR}>")
