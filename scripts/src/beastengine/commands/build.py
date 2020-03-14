import argparse
import sys

import colorama

from ..beast_command_helper import BeastCommandHelper
from ...commandrunners.cmake import CMake
from ...config.Config import Config
from ...config.ConfigNames import ConfigNames


class Build:
    DESCRIPTION = '''{yellow}beast build [<args>]

{purple}Available arguments{white}
{green}-c --config{white}   Defines the configuration in which the project will be built.
              The available configurations are: {configs}.
              When no config is specified, the default "{default_config}" value is set.
'''

    def __init__(self, config: Config, cwd):
        self.config = config
        self.cwd = cwd

        substitution_map = {
            'configs': ConfigNames.available_names(),
            'default_config': config.get_default_configuration()
        }

        parser = argparse.ArgumentParser(usage=BeastCommandHelper.format_text(self.DESCRIPTION, substitution_map))
        parser.add_argument('-c', '--config', help='what configuration should the project be built for')

        self.execute(parser.parse_args(sys.argv[2:]))

    def execute(self, command_line_args):
        if not command_line_args.config:
            config_name = self.config.get_default_configuration()

            info_message = "{yellow}No configuration specified, building for default \"{default_config}\" configuration {reset}"
            info_message = BeastCommandHelper.format_text(info_message, {'default_config': config_name})
            info_message = info_message.format(config_name)
            print(info_message)
        else:
            config_name = command_line_args.config

        try:
            config_name = ConfigNames.from_string(config_name)
            CMake.build(config_name, self.cwd)
        except ValueError:
            substitution_map = {'config': config_name, 'configs': ConfigNames.available_names()}

            message = "{red}'{config}' is not a valid configuration!\n{yellow}The available configurations are: {configs}"
            message = BeastCommandHelper.format_text(message, substitution_map)

            print(message)

