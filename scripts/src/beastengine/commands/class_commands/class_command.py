import sys

from src.beastengine.commands.configure import Configure
from src.beastengine.commands.class_commands.class_remove import ClassRemove
from src.beastengine.commands.class_commands.class_files_helper import ClassFilesHelper
from src.beastengine.commands.class_commands.class_add import ClassAdd
from src.functions import create_arguments_parser, is_verbose_set
from src.beastengine.beast_command_helper import BeastCommandHelper
from src.commandrunners.cmake.cmake import CMake
from src.config.config_manager import ConfigManager
from src.config.target_config_manager import TargetConfigManager


class ClassCommand:
    PROGRAM_USAGE = '''{green}beast {class} <target> <command> [<args>]

{white}This command operates on header and source files relatively to the project source directory defined in CMake and additionally
relative to the 'config.cmake.target.headers|sources.base_dir' parameter.

{purple}Available targets{white}
 {green}{target_lib}{white}           BeastEngine library
 {green}{target_tests}{white}         BeastEngine's Lab testing executable

{purple}Available commands{white}
 {green}{class_add}{white}           Adds new class to the given target
 {green}{class_remove}{white}        Removes existing class from the given target
 {green}{class_path_show}{white}          Displays information about base directory for headers and sources of given target 

{yellow}Type "beast {class} <target> <command> --help" for more information on a specific class related command{white}
'''

    def __init__(
        self,
        config_manager: ConfigManager,
        cmake: CMake,
        target_config_manager: TargetConfigManager,
        class_files_helper: ClassFilesHelper
    ):
        usage = BeastCommandHelper.format_text(
            self.PROGRAM_USAGE,
            {
                'target_lib': ConfigManager.TARGET_NAME_LIB,
                'target_tests': ConfigManager.TARGET_NAME_TESTS,
            }
        )
        parser = create_arguments_parser(usage=usage)
        parser.add_argument('target', help='target for which the files should be added', metavar='<target>')
        parser.add_argument('command', help='command to execute', metavar='<command>')

        command_line_arguments = parser.parse_args(sys.argv[2:4])

        target_name = command_line_arguments.target
        target_config = config_manager.get_target_config_by_name(target_name)
        if target_config is None:
            print(f'\'{target_name}\' is not a valid target!')
            return

        headers_base_directory =\
            target_config_manager.get_headers_base_directory(
                target_config,
                config_manager.config.cmake
            )

        sources_base_directory =\
            target_config_manager.get_sources_base_directory(
                target_config,
                config_manager.config.cmake
            )

        command = command_line_arguments.command
        if command == BeastCommandHelper.COMMAND_NAME_CLASS_ADD:
            ClassAdd(
                headers_base_directory,
                sources_base_directory,
                class_files_helper,
                target_config,
                config_manager
            )
        elif command == BeastCommandHelper.COMMAND_NAME_CLASS_REMOVE:
            ClassRemove(
                headers_base_directory,
                sources_base_directory,
                class_files_helper,
                target_config,
                config_manager
            )
        elif command == BeastCommandHelper.COMMAND_NAME_CLASS_PATH_SHOW:
            print(f'Headers base directory: {headers_base_directory}\nSources base directory: {sources_base_directory}')
            return

        else:
            return

        is_verbose = is_verbose_set(command_line_arguments)
        cmake.generate_configs(is_verbose)
        cmake.configure(is_verbose)
