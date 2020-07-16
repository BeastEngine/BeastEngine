import sys

from src.beastengine.commands.class_commands.class_remove import ClassRemove
from src.beastengine.commands.class_commands.class_files_helper import ClassFilesHelper
from src.beastengine.commands.class_commands.class_add import ClassAdd
from src.functions import create_arguments_parser, is_verbose_set
from src.beastengine.beast_command_helper import BeastCommandHelper
from src.commandrunners.cmake.cmake import CMake
from src.config.config_manager import ConfigManager
from src.config.target_config_manager import TargetConfigManager


class ClassCommand:
    PROGRAM_USAGE = '''{green}beast {class} <command> [<args>]

{white}This command operates on header and source files relatively to the project source directory defined in CMake and additionally
relative to the 'config.cmake.target.headers|sources.base_dir' parameter.

{purple}Available commands{white}
 {green}{class_add}{white}           Adds new class to the BeastEngine library
 {green}{class_remove}{white}        Removes existing class from the BeastEngine library
 {green}{class_path_show}{white}          Displays information about base directory for headers and sources

{yellow}Type "beast {class} <command> --help" for more information on a specific class related command{white}
'''

    def __init__(
        self,
        config_manager: ConfigManager,
        cmake: CMake,
        target_config_manager: TargetConfigManager,
        class_files_helper: ClassFilesHelper
    ):
        target_config = config_manager.config.cmake.lib
        parser = create_arguments_parser(usage=BeastCommandHelper.format_text(self.PROGRAM_USAGE))
        parser.add_argument('command', help='command to execute', metavar='<command>')

        command_line_arguments = parser.parse_args(sys.argv[2:3])

        headers_base_directory = \
            target_config_manager.get_headers_base_directory(
                target_config,
                config_manager.config.cmake
            )

        sources_base_directory = \
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

        cmake.generate_configs(is_verbose_set(command_line_arguments))
