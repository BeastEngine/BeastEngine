from .command_runner import CommandRunner
from ..config.ConfigNames import ConfigNames


class CMake:
    class Commands:
        COMMAND_INIT = "cmake -S . -B {}"
        COMMAND_BUILD = "cmake --build . --config {}"

    @staticmethod
    def init(build_dir_name: str, cwd: str):
        CommandRunner.run_command(CMake.Commands.COMMAND_INIT.format(build_dir_name), cwd)

    @staticmethod
    def build(build_type: ConfigNames, cwd: str):
        CommandRunner.run_command(CMake.Commands.COMMAND_BUILD.format(build_type), cwd)
