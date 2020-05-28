import sys
import argparse

from src.commandrunners.command_runner import CommandRunner
from src.beastengine.commands.class_command import ClassCommand
from src.beastengine.commands.init import Init
from src.beastengine.commands.configure import Configure
from src.beastengine.commands.build import Build
from src.beastengine.beast_command_helper import BeastCommandHelper
from src.commandrunners.cmake.cmake import CMake
from src.commandrunners.conan import Conan
from src.config.config_manager import ConfigManager


class BeastEngine:
    PROGRAM_NAME = "beast"
    PROGRAM_USAGE = '''{green}beast <command> [<args>]

{purple}Configuration commands{white}
 {green}{init}{white}          Installs BeastEngine. Creates 'build' directory and downloads all necessary dependencies
 {green}{configure}{white}     Configures CMake project inside the 'build' directory

{purple}Development commands{white}
 {green}{build}{white}         Builds BeastEngine project based on given parameters
 {green}{class}{white}         Performs operations on classes

{yellow}Type "beast <command> --help" for more information on a specific command{white}
'''
    PROGRAM_DESCRIPTION = '''
{white}This program let's you configure and manage the BeastEngine project.
Use it to install all the required dependencies and configure CMake project.
You can also use it for building the project with desired configuration.{reset}'''

    def __init__(
            self,
            project_working_dir,
            build_dir,
            command_runner: CommandRunner,
            config_manager: ConfigManager,
            conan: Conan,
            cmake: CMake
    ):
        self.config_manager = config_manager
        self.command_runner = command_runner

        self.project_dir = project_working_dir
        self.build_dir = build_dir

        self.conan = conan
        self.cmake = cmake

        self.create_program()

    def create_program(self):
        parser = \
            argparse.ArgumentParser(
                prog=self.PROGRAM_NAME,
                usage=BeastCommandHelper.format_text(self.PROGRAM_USAGE),
                description=BeastCommandHelper.format_text(self.PROGRAM_DESCRIPTION),
                formatter_class=argparse.RawDescriptionHelpFormatter
            )

        parser.add_argument('command', help="command to execute", metavar='<command>')
        command_line_args = parser.parse_args(sys.argv[1:2])

        if command_line_args.command == BeastCommandHelper.COMMAND_NAME_INIT:
            Init(self.project_dir, self.command_runner, self.conan, self.cmake)
        elif command_line_args.command == BeastCommandHelper.COMMAND_NAME_CONFIGURE:
            Configure(self.cmake)
        elif command_line_args.command == BeastCommandHelper.COMMAND_NAME_BUILD:
            Build(self.config_manager, self.cmake).execute()
        elif command_line_args.command == BeastCommandHelper.COMMAND_NAME_CLASS:
            # file = open(self.project_dir + '/src/CMakeLists.txt')
            # print(file.read().find("BEAST_HEADERS_LIST"))
            # file.close()
            # self.cmake.__generate_target_headers_and_sources()
            print('eldo mondo')
            class_command = ClassCommand(self.config_manager, self.cmake)
            self.config_manager.update_config()
            class_command.add_class("cyp")
