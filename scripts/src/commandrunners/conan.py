from src.commandrunners.command_runner import CommandRunner
from src.config.config_names import ConfigNames


class Conan:
    COMMAND_INSTALL = "conan install .. -g cmake_multi --build=missing -s build_type="

    def __init__(self, command_runner: CommandRunner, build_dir: str):
        self.command_runner = command_runner
        self.build_dir = build_dir

    def install(self, verbose):
        for name, member in ConfigNames.__members__.items():
            self.command_runner.run_command(Conan.COMMAND_INSTALL + member.value, self.build_dir, verbose)
