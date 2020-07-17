from src.commandrunners.cmake.cmake_config_files_creator import CMakeConfigFilesCreator
from src.commandrunners.command_runner import CommandRunner
from src.config.config_manager import ConfigManager, Config
from src.config.config_names import ConfigNames


class CMake:
    COMMAND_INIT = 'cmake -S . -B {}'
    COMMAND_BUILD = 'cmake --build . --config {}'

    FILE_MODE_READ = 'r'
    FILE_MODE_READ_APPEND_WRITE = 'r+'
    FILE_MODE_TRUNCATE_WRITE = 'w'

    def __init__(
            self,
            command_runner: CommandRunner,
            config_files_creator: CMakeConfigFilesCreator,
            general_config_manager: ConfigManager,
            project_dir: str,
            build_dir: str
    ):
        self.command_runner = command_runner
        self.config_files_creator = config_files_creator

        self.project_dir = project_dir
        self.build_dir = build_dir

        self.config = general_config_manager.config.cmake
        self.config_dir = f'{self.project_dir}/{self.config.directory_name}'

    def configure(self, verbose: bool):
        self.command_runner.run_command(CMake.COMMAND_INIT.format(self.build_dir), self.project_dir, verbose)

    def build(self, config_name: ConfigNames, verbose: bool):
        self.command_runner.run_command(CMake.COMMAND_BUILD.format(config_name), self.build_dir, verbose)

    def generate_configs(self, verbose: bool):
        self.generate_main_config(verbose)
        self.generate_targets_configs(verbose)

    def generate_main_config(self, verbose: bool):
        self.config_files_creator.generate_main_config(self.config, self.config_dir, verbose)

    def generate_targets_configs(self, verbose: bool):
        self.generate_target_config(self.config.lib, verbose)
        self.generate_target_config(self.config.exe, verbose)

    def generate_target_config(self, target: Config.CMake.Target, verbose: bool):
        self.config_files_creator.generate_target_config(target, self.config_dir, verbose)
