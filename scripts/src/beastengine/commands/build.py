import sys

from src.functions import create_arguments_parser, is_verbose_set
from src.beastengine.beast_command_helper import BeastCommandHelper
from src.commandrunners.cmake.cmake import CMake
from src.config.config_manager import ConfigManager
from src.config.config_names import ConfigNames


class Build:
    DESCRIPTION = '''{green}beast build [-c|--config CONFIG <args>]

{purple}Available arguments{white}
{green}-c --config{white}   Defines the configuration in which the project will be built.
              The available configurations are: {configs}.
              If no config is specified, the build is performed for all configurations at once.
'''

    NO_CONFIG_INFO_MESSAGE_TEMPLATE =\
        "{yellow}No configuration specified, building for all configurations {reset}"
    INVALID_CONFIG_ERROR_MESSAGE_TEMPLATE =\
        "{red}'{config}' is not a valid configuration!\n{yellow}The available configurations are: {configs}"

    def __init__(self, config_manager: ConfigManager, cmake: CMake):
        self.config_manager = config_manager
        self.cmake = cmake

        substitution_map = {'configs': ConfigNames.available_names()}

        parser = create_arguments_parser(usage=BeastCommandHelper.format_text(self.DESCRIPTION, substitution_map))
        parser.add_argument('-c', '--config', help='what configuration should the project be built for. Leave empty for all at once build ')

        self.command_line_arguments = parser.parse_args(sys.argv[2:])
        self.is_verbose = is_verbose_set(self.command_line_arguments)

    def execute(self):
        if not self.command_line_arguments.config:
            info_message = BeastCommandHelper.format_text(self.NO_CONFIG_INFO_MESSAGE_TEMPLATE)
            print(info_message)

            for config in ConfigNames.all_configs():
                self.cmake.build(config, self.is_verbose)

            return

        config_name = self.command_line_arguments.config
        try:
            config_name = ConfigNames.from_string(config_name)
            self.cmake.build(config_name, self.is_verbose)
        except ValueError:
            substitution_map = {'config': config_name, 'configs': ConfigNames.available_names()}
            message = BeastCommandHelper.format_text(self.INVALID_CONFIG_ERROR_MESSAGE_TEMPLATE, substitution_map)
            print(message)
