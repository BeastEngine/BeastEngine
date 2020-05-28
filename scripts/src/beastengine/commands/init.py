import sys

from src.commandrunners.cmake.cmake import CMake
from src.commandrunners.command_runner import CommandRunner
from src.commandrunners.conan import Conan
from src.functions import get_build_dir_name, create_arguments_parser, is_verbose_set


class Init:
    def __init__(self, project_dir, command_runner: CommandRunner, conan: Conan, cmake: CMake):
        build = get_build_dir_name()
        command_remove_build_dir = "rm -rf " + build
        command_create_build_dir = "mkdir " + build

        parser = create_arguments_parser()
        args = parser.parse_args(sys.argv[2:])
        should_be_verbose = is_verbose_set(args)

        command_runner.run_command(command_remove_build_dir, project_dir, should_be_verbose)
        command_runner.run_command(command_create_build_dir, project_dir, should_be_verbose)

        conan.install(should_be_verbose)
        cmake.generate_configs(should_be_verbose)
