import glob
import os
import subprocess

TEST_DIR = "test/"

C_NAME = "C"
LL_NAME = "LL_compiled"


def generate_ll():
    for file in glob.glob(f"{TEST_DIR}{C_NAME}/**/*.c", recursive=True):
        target_file = file \
            .replace(C_NAME, LL_NAME) \
            .replace(".c", ".ll")
        subprocess.call(["clang", "-S", "-emit-llvm", file, "-o", target_file])


def copy_dir_structure():
    for dir_path, dir_names, filenames in os.walk(f"{TEST_DIR}{C_NAME}/"):
        structure = os.path.join(f"{TEST_DIR}{LL_NAME}/", dir_path[len(f"{TEST_DIR}{C_NAME}/"):])
        if not os.path.isdir(structure):
            os.mkdir(structure)


def clean():
    subprocess.call(["rm", "-rf", f"{TEST_DIR}{LL_NAME}"])


if __name__ == '__main__':
    clean()
    copy_dir_structure()
    generate_ll()
