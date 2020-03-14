import sys
import argparse
import colorama

from .commands.build import Build
from .commands.configure import Configure
from .beast_command_helper import BeastCommandHelper
from ..config.Config import Config
from .commands.init import Init


class BeastEngine:
    PROGRAM_NAME = "beast"
    PROGRAM_USAGE = '''{yellow}beast <command> [<args>]

{purple}Project commands{white}        
 {green}{init}{white}          Installs BeastEngine. Creates 'build' directory and downloads all necessary dependencies
 {green}{configure}{white}     Configures CMake project inside the 'build' directory
 {green}{build}{white}         Builds BeastEngine project based on given parameters

{yellow}Type "beast <command> --help" for more information on a specific command{white}
'''
    PROGRAM_DESCRIPTION = '''
{white}This program let's you configure and manage the BeastEngine project.
Use it to install all the required dependencies and configure CMake project.
You can also use it for building the project with desired configuration.{reset}'''

    def __init__(self, project_working_dir):
        self.config = Config()

        self.cwd = project_working_dir
        self.build_dir = self.cwd + '/' + BeastCommandHelper.DIRECTORY_BUILD

        self.create_program()

    def create_program(self):
        parser = \
            argparse.ArgumentParser(
                prog=self.PROGRAM_NAME,
                usage=BeastCommandHelper.format_text(self.PROGRAM_USAGE),
                description=BeastCommandHelper.format_text(self.PROGRAM_DESCRIPTION),
                formatter_class=argparse.RawDescriptionHelpFormatter
            )

        parser.add_argument('command', help="Command to execute", metavar='<command>')
        command_line_args = parser.parse_args(sys.argv[1:2])

        if command_line_args.command == BeastCommandHelper.COMMAND_NAME_INIT:
            Init(self.cwd, self.build_dir)
        elif command_line_args.command == BeastCommandHelper.COMMAND_NAME_CONFIGURE:
            Configure(self.cwd)
        elif command_line_args.command == BeastCommandHelper.COMMAND_NAME_BUILD:
            Build(self.config, self.build_dir)
