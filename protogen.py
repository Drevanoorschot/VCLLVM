import os.path
import shutil
import subprocess

PROTO_SRC_DIR = "lib/AST/proto"
PROTO_INCLUDE_DIR = "include/AST/proto"
TEMP_GEN_DIR = "temp"


def protogen():
    # create include folder if not present
    if not os.path.isdir(PROTO_INCLUDE_DIR):
        os.mkdir(PROTO_INCLUDE_DIR)
    # make temp directory for generated files
    os.mkdir(TEMP_GEN_DIR)
    for file in filter(lambda f: f.endswith(".proto"), os.listdir(PROTO_SRC_DIR)):
        subprocess.call(['protoc', f"--cpp_out={TEMP_GEN_DIR}", f"{PROTO_SRC_DIR}/{file}"])
        shutil.copy(f"{TEMP_GEN_DIR}/{PROTO_SRC_DIR}/{file.replace('.proto', '.pb.h')}", f"{PROTO_INCLUDE_DIR}")
    # remove temp folder
    shutil.rmtree(TEMP_GEN_DIR)


def clean():
    if os.path.isdir(PROTO_INCLUDE_DIR):
        shutil.rmtree(PROTO_INCLUDE_DIR)


"""
This script generates protobuf header files for *.proto files located in lib/AST/proto and puts them in the
include/AST/proto directory.
"""
if __name__ == '__main__':
    clean()
    protogen()
