import argparse
import sys

from src.beastengine.beast_command_helper import BeastCommandHelper
from src.commandrunners.cmake.cmake import CMake
from src.config.config_manager import ConfigManager


class ClassCommand:
    PROGRAM_USAGE = '''{green}beast {class} <command> [<args>]

{purple}Available commands{white}
 {green}{class_add}{white}           Adds new class to the BeastEngine library
 {green}{class_remove}{white}        Removes existing class from the BeastEngine library

{yellow}Type "beast class <command> --help" for more information on a specific class related command{white}
'''

    def __init__(self, config_manager: ConfigManager, cmake: CMake):
        self.config_manager = config_manager
        self.cmake = cmake

        # Possible options are:
        # --add
            # --header_only
            # --source_only
        # --remove
        # <class_name>
        usage = BeastCommandHelper.format_text(self.PROGRAM_USAGE)
        parser = argparse.ArgumentParser(usage=usage)
        parser.parse_args(sys.argv[2:])

    def add_class(self, class_name: str):

        # stdout = CommandRunner.get_command_output("grep BEAST_HEADERS_DIR build/CMakeCache.txt")
        # print(stdout.split('=')[1])
        self.cmake.generate_main_config()
        # TODO: Check if file(s) already exist(s)
        # TODO: Create file(s)
        # TODO: Add new file(s) to the config
        # TODO: Regenerate CMake configs
        pass

