add_library(VcLLVM_PASSES SHARED
        FunctionDeclarer.cpp
        VariableNamer.cpp)
target_include_directories(VcLLVM_PASSES PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../include")