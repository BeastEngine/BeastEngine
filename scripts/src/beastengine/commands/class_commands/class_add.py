import sys

from src.beastengine.beast_command_helper import BeastCommandHelper
from src.beastengine.commands.class_commands.class_files_helper import ClassFilesHelper
from src.functions import create_arguments_parser, is_verbose_set
from src.config.config_manager import Config, ConfigManager


class ClassAdd:
    PROGRAM_USAGE = '''{green}beast {class} {class_add} <class_name> [<args>]

{white}This command creates single header and single source files under the headers and sources base directories of the given CMake target.
If class name contains slashes, it will create subdirectories inside base directory.
Eg. {yellow}beast {class} {class_add} subDir/myClass{white} will result in creation of the 'myClass.h' and 'myClass.cpp'
files under the 'baseDirectory/subDir' path.{white}
'''

    CLASS_EXISTS_ERROR_MESSAGE_TEMPLATE = "'{}' class already exists!"

    def __init__(
            self,
            headers_base_dir: str,
            sources_base_dir: str,
            class_files_helper: ClassFilesHelper,
            target_config: Config.CMake.Target,
            config_manager: ConfigManager
    ):
        parser = create_arguments_parser(usage=BeastCommandHelper.format_text(self.PROGRAM_USAGE))
        parser.add_argument('class_name', help='class to add', metavar='<class_name>')
        parser.add_argument('-n', '--namespace', help='namespace in which the class should reside', type=str)

        group = parser.add_mutually_exclusive_group()
        group.add_argument('-ho', '--header_only', help='create only header file and omit the source file', action='store_true')
        group.add_argument('-so', '--source_only', help='create only source file and omit the header file', action='store_true')

        command_line_arguments = parser.parse_args(sys.argv[4:])
        is_verbose = is_verbose_set(command_line_arguments)

        class_name = command_line_arguments.class_name
        if class_files_helper.class_files_exist(target_config, class_name):
            print(self.CLASS_EXISTS_ERROR_MESSAGE_TEMPLATE.format(class_name))
            return

        namespace = None
        if command_line_arguments:
            namespace = command_line_arguments.namespace

        if command_line_arguments.header_only:
            class_files_helper.create_class_header(class_name, headers_base_dir, is_verbose, namespace)
            target_config.headers.files.append(class_files_helper.get_header_file_name(class_name))
        elif command_line_arguments.source_only:
            class_files_helper.create_class_source(class_name, sources_base_dir, is_verbose, namespace)
            target_config.sources.files.append(class_files_helper.get_source_file_name(class_name))
        else:
            class_files_helper.create_class_files(class_name, headers_base_dir, sources_base_dir, is_verbose, namespace)
            target_config.headers.files.append(class_files_helper.get_header_file_name(class_name))
            target_config.sources.files.append(class_files_helper.get_source_file_name(class_name))

        config_manager.update_config()
