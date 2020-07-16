import sys

from src.functions import create_arguments_parser, is_verbose_set
from src.commandrunners.cmake.cmake import CMake


class Configure:
    def __init__(self, cmake: CMake):
        parser = create_arguments_parser()
        arguments = parser.parse_args(sys.argv[2:])

        is_verbose = is_verbose_set(arguments)

        cmake.generate_configs(is_verbose)
        cmake.configure(is_verbose)
