import sys

from src.beastengine.beast_command_helper import BeastCommandHelper
from src.beastengine.commands.class_commands.class_files_helper import ClassFilesHelper
from src.functions import create_arguments_parser, is_verbose_set
from src.config.config import Config
from src.config.target_config_manager import TargetConfigManager


class ClassAdd:
    PROGRAM_USAGE = '''{green}beast {class} {class_add} <target> <class_name> [<args>]

{white}This command creates single header and single source files under the headers and sources base directories of the given CMake target.
If class name contains slashes, it will create subdirectories inside base directory.
Eg. {yellow}beast {class} {class_add} subDir/myClass{white} will result in creation of the 'myClass.h' and 'myClass.cpp'
files under the 'baseDirectory/subDir' path.{white}
'''

    CLASS_SOURCE_FILE_EXISTS_ERROR_MESSAGE_TEMPLATE = "'{}' source file already exists!"
    CLASS_HEADER_FILE_EXISTS_ERROR_MESSAGE_TEMPLATE = "'{}' header file already exists!"
    CLASS_EXISTS_ERROR_MESSAGE_TEMPLATE = "'{}' class already exists!"

    def __init__(
            self,
            target_config_manager: TargetConfigManager,
            class_files_helper: ClassFilesHelper,
            config: Config
    ):
        parser = create_arguments_parser(usage=BeastCommandHelper.format_text(self.PROGRAM_USAGE))
        parser.add_argument('target', help='target for which the files should be added', metavar='<target>')
        parser.add_argument('class_name', help='class to add', metavar='<class_name>')
        parser.add_argument('-n', '--namespace', help='namespace in which the class should reside', type=str)

        group = parser.add_mutually_exclusive_group()
        group.add_argument('-ho', '--header_only', help='create only header file and omit the source file', action='store_true')
        group.add_argument('-so', '--source_only', help='create only source file and omit the header file', action='store_true')

        command_line_arguments = parser.parse_args(sys.argv[3:])
        is_verbose = is_verbose_set(command_line_arguments)

        class_name = command_line_arguments.class_name
        target_name = command_line_arguments.target
        try:
            target_config = config.cmake['targets'][target_name]
        except KeyError:
            raise RuntimeError(f'\'{target_name}\' is not a valid target!')

        headers_base_dir = target_config_manager.get_headers_base_directory(target_config, config.cmake)
        sources_base_dir = target_config_manager.get_sources_base_directory(target_config, config.cmake)

        namespace = None
        if command_line_arguments:
            namespace = command_line_arguments.namespace

        if command_line_arguments.header_only:
            if class_files_helper.class_header_file_exist(target_config, class_name):
                print(self.CLASS_EXISTS_ERROR_MESSAGE_TEMPLATE.format(class_name))
                return

            class_files_helper.create_class_header(class_name, headers_base_dir, is_verbose, namespace)
            target_config['headers']['files'].append(class_files_helper.get_header_file_name(class_name))
        elif command_line_arguments.source_only:
            if class_files_helper.class_source_file_exist(target_config, class_name):
                print(self.CLASS_SOURCE_FILE_EXISTS_ERROR_MESSAGE_TEMPLATE.format(class_name))
                return

            class_files_helper.create_class_source(class_name, sources_base_dir, is_verbose, namespace)
            target_config['sources']['files'].append(class_files_helper.get_source_file_name(class_name))
        else:
            if class_files_helper.class_files_exist(target_config, class_name):
                print(self.CLASS_EXISTS_ERROR_MESSAGE_TEMPLATE.format(class_name))
                return

            class_files_helper.create_class_files(class_name, headers_base_dir, sources_base_dir, is_verbose, namespace)
            target_config['headers']['files'].append(class_files_helper.get_header_file_name(class_name))
            target_config['sources']['files'].append(class_files_helper.get_source_file_name(class_name))

        config.update()
