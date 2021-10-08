from typing import List

import os
import glob


def remove_if_exists(filename: str) -> None:
    if os.path.exists(filename):
        os.remove(filename)


def list_test_files() -> List[str]:
    return [file for file in os.listdir() if file.endswith(".shtk")]


def get_files(file_ext: str) -> List[str]:
    return glob.glob(f"**/*{file_ext}", recursive=True)


def remove_files(file_ext: str) -> None:
    files = get_files(file_ext)

    for file in files:
        remove_if_exists(file)


def remove_test_executable() -> None:
    remove_if_exists("./test", )
