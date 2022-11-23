add_library(AST SHARED
        AST.cpp)
target_include_directories(AST PRIVATE  "${CMAKE_CURRENT_SOURCE_DIR}/../../include")