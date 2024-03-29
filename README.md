# VCLLVM

![logo](https://i.imgur.com/ERcNUPV.png)

This project was part of the following [Master Thesis](https://essay.utwente.nl/96536/).

## Requirements
- Debian 11 (Most other Linux distros will probably work fine too, but are untested)
- CMake (>=3.4.3)
- LLVM (15.0.7)
- Protobuf (3.22.0)

## Building
To build the project with ninja, run the following:
```bash
$ cmake -G ninja -B .build
$ cmake --build .build --target VCLLVM
```
Run with:
```bash
$ ./_build/bin/VCLLVM <input_file>
```
Installation is required to be used alongside [VerCors](https://github.com/utwente-fmt/vercors):
```bash
$ cmake --install .build
```

## Testing
Regression testing is implemented using LLVM 
[lit](https://llvm.org/docs/CommandGuide/lit.html#local-configuration-files).
This requires Python (>=3.9) to be installed as well as the lit package:
```bash
$ pip install lit==15.0.6
```
Furthermore, to compile the tests, clang (15.0.6) should be installed. Running the test depends on the build system being used. We'll use ninja as an example. Once the project is build, go
into the build directory and run:
```bash
$ ninja check
```
The regression tests only test whether VCLLVM crashes on any inputs. It does not attempt to test correctness of the 
output.
