
set(PROTOFILES proto/ast.proto)

find_package(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTOFILES})

add_library(AST SHARED ${PROTO_SRCS} ${PROTO_HDRS} AST.cpp)

target_link_libraries(AST ${Protobuf_LIBRARIES})

target_include_directories(AST PRIVATE "${PROJECT_SOURCE_DIR}/include")
