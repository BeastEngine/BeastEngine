import sys
import argparse

from src.beastengine.commands.class_commands.class_files_helper import ClassFilesHelper
from src.config.target_config_manager import TargetConfigManager
from src.commandrunners.command_runner import CommandRunner
from src.beastengine.commands.class_commands.class_command import ClassCommand
from src.beastengine.commands.init import Init
from src.beastengine.commands.configure import Configure
from src.beastengine.commands.build import Build
from src.beastengine.beast_command_helper import BeastCommandHelper
from src.commandrunners.cmake.cmake import CMake
from src.commandrunners.conan import Conan
from src.config.config import Config


class BeastEngine:
    PROGRAM_NAME = 'beast'
    PROGRAM_USAGE = '''{green}beast <command> [<args>]

{white}This program let's you configure and manage the BeastEngine project.
Use it to install all the required dependencies and configure CMake project.
You can also use it for building the project with desired configuration.{reset}

{purple}Configuration commands{white}
 {green}{init}{white}          Installs BeastEngine. Creates 'build' directory and downloads all necessary dependencies
 {green}{configure}{white}     Configures CMake project inside the 'build' directory
 {green}{install_deps}{white}       Installs and/or updates project dependencies 

{purple}Development commands{white}
 {green}{build}{white}         Builds BeastEngine project based on given parameters
 {green}{class}{white}         Performs operations on classes

{yellow}Type "beast <command> --help" for more information on a specific command{white}
'''

    def __init__(
            self,
            project_working_dir,
            build_dir,
            command_runner: CommandRunner,
            config: Config,
            conan: Conan,
            cmake: CMake,
            target_config_manager: TargetConfigManager,
            class_files_helper: ClassFilesHelper
    ):
        self.config = config
        self.command_runner = command_runner

        self.project_dir = project_working_dir
        self.build_dir = build_dir

        self.conan = conan
        self.cmake = cmake

        self.target_config_manager = target_config_manager
        self.class_files_helper = class_files_helper

        self.create_program()

    def create_program(self):
        parser =\
            argparse.ArgumentParser(
                prog=self.PROGRAM_NAME,
                usage=BeastCommandHelper.format_text(self.PROGRAM_USAGE),
                formatter_class=argparse.RawDescriptionHelpFormatter
            )

        parser.add_argument('command', help='command to execute', metavar='<command>')
        command_line_args = parser.parse_args(sys.argv[1:2])

        if command_line_args.command == BeastCommandHelper.COMMAND_NAME_INIT:
            Init(self.project_dir, self.command_runner, self.conan, self.cmake)
        elif command_line_args.command == BeastCommandHelper.COMMAND_NAME_CONFIGURE:
            Configure(self.cmake)
        elif command_line_args.command == BeastCommandHelper.COMMAND_NAME_BUILD:
            Build(self.config, self.cmake).execute()
        elif command_line_args.command == BeastCommandHelper.COMMAND_NAME_CLASS:
            ClassCommand(self.config, self.cmake, self.target_config_manager, self.class_files_helper)
        elif command_line_args.command == BeastCommandHelper.COMMAND_NAME_INSTALL_DEPENDENCIES:
            self.conan.install(True)
