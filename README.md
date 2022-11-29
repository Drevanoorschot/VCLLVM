# VcLLVM

## Requirements
- CMake (>=3.4.3)
- LLVM (15.0.6)
- Protobuf (3.21.9)
- Python (>=3.9)

In order to use the `llgen.py` script, you will also need:
- Clang (15.0.6)
## Development Notes
- All proto files should go in `lib/AST/proto` directly.
- Make sure to run `protogen.py` before building the project for the first
time or when changing/adding any `*.proto` file in `lib/AST/proto`.