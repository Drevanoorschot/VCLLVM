find_package(Protobuf REQUIRED PATHS /usr/local/lib/cmake/protobuf)

add_library(proto-objects OBJECT "col.proto")

target_link_libraries(proto-objects PUBLIC protobuf::libprotobuf)

file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/generated")

set(PROTO_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated")

target_include_directories(proto-objects PUBLIC "$<BUILD_INTERFACE:${PROTO_BINARY_DIR}>")

protobuf_generate(
        TARGET proto-objects
        TARGET_DIR "."
        PROTOC_OUT_DIR "${PROTO_BINARY_DIR}")