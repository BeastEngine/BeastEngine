import sys

from src.functions import create_arguments_parser, is_verbose_set
from src.commandrunners.cmake.cmake import CMake


class Configure:
    def __init__(self, cmake: CMake):
        parser = create_arguments_parser()
        arguments = parser.parse_args(sys.argv[2:])

        cmake.configure(is_verbose_set(arguments))
