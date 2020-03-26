from .command_runner import CommandRunner
from ..config.ConfigNames import ConfigNames


class Conan:
    COMMAND_INSTALL = "conan install .. -g cmake_multi --build=missing -s build_type="

    def __init__(self, build_dir):
        self.build_dir = build_dir

    def install(self):
        for name, member in ConfigNames.__members__.items():
            CommandRunner.run_command(Conan.COMMAND_INSTALL + member.value, self.build_dir)
