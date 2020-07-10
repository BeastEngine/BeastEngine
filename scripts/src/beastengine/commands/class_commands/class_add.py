import sys

from src.beastengine.commands.class_commands.class_files_helper import ClassFilesHelper
from src.functions import create_arguments_parser, is_verbose_set
from src.config.config_manager import Config, ConfigManager


class ClassAdd:
    CLASS_EXISTS_ERROR_MESSAGE_TEMPLATE = "'{}' class already exists!"

    def __init__(
            self,
            headers_base_dir: str,
            sources_base_dir: str,
            class_files_helper: ClassFilesHelper,
            target_config: Config.CMake.Target,
            config_manager: ConfigManager
    ):
        parser = create_arguments_parser()
        parser.add_argument('class_name', help='class to add', metavar='<class_name>')
        parser.add_argument('-n', '--namespace', help='namespace in which the class should reside', type=str)

        command_line_arguments = parser.parse_args(sys.argv[3:])
        is_verbose = is_verbose_set(command_line_arguments)

        class_name = command_line_arguments.class_name
        if class_files_helper.class_files_exist(target_config, class_name):
            print(self.CLASS_EXISTS_ERROR_MESSAGE_TEMPLATE.format(class_name))
            return

        namespace = None
        if command_line_arguments:
            namespace = command_line_arguments.namespace

        class_files_helper.create_class_files(class_name, headers_base_dir, sources_base_dir, is_verbose, namespace)
        target_config.headers.files.append(class_files_helper.get_header_file_name(class_name))
        target_config.sources.files.append(class_files_helper.get_source_file_name(class_name))

        config_manager.update_config()
