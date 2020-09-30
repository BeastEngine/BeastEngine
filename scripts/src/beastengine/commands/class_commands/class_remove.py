import sys

from src.beastengine.beast_command_helper import BeastCommandHelper
from src.functions import create_arguments_parser, is_verbose_set
from src.beastengine.commands.class_commands.class_files_helper import ClassFilesHelper
from src.config.config import Config
from src.config.target_config_manager import TargetConfigManager


class ClassRemove:
    PROGRAM_USAGE = '''{green}beast {class} {class_remove} <class_name> [<args>]

{white}This command removes header and source files from the headers and sources base directories.
If class name contains slashes, it will also delete empty subdirectories inside base directory.
'''

    def __init__(self, target_config_manager: TargetConfigManager, class_files_helper: ClassFilesHelper, config: Config):
        parser = create_arguments_parser(usage=BeastCommandHelper.format_text(self.PROGRAM_USAGE))
        parser.add_argument('target', help='target from which the files should be removed', metavar='<target>')
        parser.add_argument('class_name', help='class to remove', metavar='<class_name>')

        command_line_arguments = parser.parse_args(sys.argv[3:])
        is_verbose = is_verbose_set(command_line_arguments)

        target_name = command_line_arguments.target
        try:
            target_config = config.cmake['targets'][target_name]
        except KeyError:
            raise RuntimeError(f'\'{target_name}\' is not a valid target!')

        headers_base_dir = target_config_manager.get_headers_base_directory(target_config, config.cmake)
        sources_base_dir = target_config_manager.get_sources_base_directory(target_config, config.cmake)

        class_name = command_line_arguments.class_name
        if class_files_helper.class_files_exist(target_config, class_name) is False:
            return

        class_files_helper.delete_class_files(class_name, headers_base_dir, sources_base_dir, is_verbose)

        header_file_name = class_files_helper.get_header_file_name(class_name)
        if target_config['headers']['files'].__contains__(header_file_name):
            target_config['headers']['files'].remove(header_file_name)

        source_file_name = class_files_helper.get_source_file_name(class_name)
        if target_config['sources']['files'].__contains__(source_file_name):
            target_config['sources']['files'].remove(source_file_name)

        config.update()
