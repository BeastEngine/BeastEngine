import sys

from src.beastengine.beast_command_helper import BeastCommandHelper
from src.functions import create_arguments_parser
from src.config.config import Config
from src.config.target_config_manager import TargetConfigManager


class ClassShowPaths:
    PROGRAM_USAGE = '''{green}beast {class} {class_path_show} <target> [<args>]

{white}Displays information about base directory for headers and sources of given target{white}
'''

    def __init__(self, target_config_manager: TargetConfigManager, config: Config):
        parser = create_arguments_parser(usage=BeastCommandHelper.format_text(self.PROGRAM_USAGE))
        parser.add_argument('target', help='target which paths to display', metavar='<target>')

        command_line_arguments = parser.parse_args(sys.argv[3:])

        target_name = command_line_arguments.target
        try:
            target_config = config.cmake['targets'][target_name]
        except KeyError:
            raise RuntimeError(f'\'{target_name}\' is not a valid target!')

        headers_base_dir = target_config_manager.get_headers_base_directory(target_config, config.cmake)
        sources_base_dir = target_config_manager.get_sources_base_directory(target_config, config.cmake)

        print(f'Headers base directory: {headers_base_dir}\nSources base directory: {sources_base_dir}')

