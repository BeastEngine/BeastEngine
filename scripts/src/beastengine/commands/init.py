import argparse
import sys

from ..beast_command_helper import BeastCommandHelper
from ...commandrunners.cmake import CMake
from ...commandrunners.command_runner import CommandRunner
from ...commandrunners.conan import Conan


class Init:
    COMMAND_REMOVE_BUILD_DIR = "rm -rf " + BeastCommandHelper.DIRECTORY_BUILD
    COMMAND_CREATE_BUILD_DIR = "mkdir " + BeastCommandHelper.DIRECTORY_BUILD

    def __init__(self, project_dir, conan: Conan, cmake: CMake):
        parser = argparse.ArgumentParser()
        parser.parse_args(sys.argv[2:])

        CommandRunner.run_command(self.COMMAND_REMOVE_BUILD_DIR, project_dir)
        CommandRunner.run_command(self.COMMAND_CREATE_BUILD_DIR, project_dir)

        conan.install()
        cmake.generate_configs()
