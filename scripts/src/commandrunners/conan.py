from .command_runner import CommandRunner
from ..config.ConfigNames import ConfigNames


class Conan:
    class Commands:
        COMMAND_INSTALL = "conan install .. -g cmake_multi --build=missing -s build_type="

    @staticmethod
    def install(working_dir: str):
        for name, member in ConfigNames.__members__.items():
            CommandRunner.run_command(Conan.Commands.COMMAND_INSTALL + member.value, working_dir)
