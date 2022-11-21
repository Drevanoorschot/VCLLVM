set(VcLLVM_PASSES
        FunctionListToErr
        InstructionMapper
        )
set(FunctionListToErr_SOURCES FunctionListToErr.cpp)
set(InstructionMapper_SOURCES InstructionMapper.cpp)

foreach (pass ${VcLLVM_PASSES})
    add_library(${pass} SHARED ${${pass}_SOURCES})
    target_include_directories(
            ${pass}
            PRIVATE
            "${CMAKE_CURRENT_SOURCE_DIR}/../include"
    )
endforeach ()