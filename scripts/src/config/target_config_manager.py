import re

from src.config.config_manager import Config
from src.beastengine.commands.class_commands.target_cmake_vars_file_opener import TargetCMakeVarsFileOpener


class TargetConfigManager:
    EXCEPTION_MESSAGE_TEMPLATE = "'{}' variable could not be found in '{}' target cmake variables file!"

    def __init__(self, file_opener: TargetCMakeVarsFileOpener):
        self.file_opener = file_opener
        self.cmake_var_pattern = re.compile(r"\${[a-zA-Z0-9._-]+\}")

    def get_headers_base_directory(self, target_config: Config.CMake.Target, cmake_config: Config.CMake):
        return self.__get_files_base_directory(target_config, cmake_config, target_config.headers)

    def get_sources_base_directory(self, target_config: Config.CMake.Target, cmake_config: Config.CMake):
        return self.__get_files_base_directory(target_config, cmake_config, target_config.sources)

    def __get_files_base_directory(
            self,
            target_config: Config.CMake.Target,
            cmake_config: Config.CMake,
            files: Config.CMake.Target.Files
    ):
        variables = self.file_opener.open(cmake_config, target_config)

        base_dir = files.base_dir
        if not base_dir:
            return ''

        matches = self.cmake_var_pattern.findall(base_dir)
        for match in matches:
            if not variables.__contains__(match):
                raise ValueError(self.EXCEPTION_MESSAGE_TEMPLATE.replace(match, target_config.target_name))

            base_dir = base_dir.replace(match, variables[match])

        return base_dir
