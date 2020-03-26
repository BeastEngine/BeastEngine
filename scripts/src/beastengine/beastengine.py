import sys
import argparse

from .commands.classcommand import ClassCommand
from .commands.init import Init
from .commands.configure import Configure
from .commands.build import Build
from .beast_command_helper import BeastCommandHelper
from ..commandrunners.cmake import CMake
from ..commandrunners.conan import Conan
from ..config.ConfigManager import ConfigManager


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

    def __init__(self, project_working_dir):
        self.config_manager = ConfigManager()

        self.project_dir = project_working_dir
        self.build_dir = self.project_dir + '/' + BeastCommandHelper.DIRECTORY_BUILD

        self.conan = Conan(self.build_dir)
        self.cmake = CMake(self.config_manager, self.project_dir, self.build_dir)

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
            Init(self.project_dir, self.conan, self.cmake)
        elif command_line_args.command == BeastCommandHelper.COMMAND_NAME_CONFIGURE:
            Configure(self.project_dir)
        elif command_line_args.command == BeastCommandHelper.COMMAND_NAME_BUILD:
            Build(self.config_manager, self.build_dir)
        elif command_line_args.command == BeastCommandHelper.COMMAND_NAME_CLASS:
            # file = open(self.project_dir + '/src/CMakeLists.txt')
            # print(file.read().find("BEAST_HEADERS_LIST"))
            # file.close()
            # self.cmake.__generate_target_headers_and_sources()
            print('eldo mondo')
            class_command = ClassCommand(self.config_manager, self.cmake)
            self.config_manager.update_config()
            class_command.add_class("cyp")
