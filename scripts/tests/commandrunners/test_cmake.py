import unittest
from unittest.mock import MagicMock, PropertyMock

from scripts.src.commandrunners.cmake import CMake
from scripts.src.commandrunners.command_runner import CommandRunner
from scripts.src.config.ConfigManager import ConfigManager, Config


class TestCMake(unittest.TestCase):
    def test_configure_will_run_proper_configuration_command(self):
        command_runner_mock = MagicMock(CommandRunner)
        command_runner_mock.run_command = MagicMock()

        config_manager_mock = MagicMock(ConfigManager)
        type(config_manager_mock).config = self.__get_empty_config()

        project_dir = "project/dir"
        build_dir = "build/dir"

        expected_command = f"cmake -S . -B {build_dir}"
        expected_cwd = project_dir

        sut = CMake(command_runner_mock, config_manager_mock, project_dir, build_dir)
        sut.configure()

        command_runner_mock.run_command.assert_called_with(expected_command, expected_cwd)

    @staticmethod
    def __get_empty_config():
        config = Config()
        config.cmake = Config.CMake()
        config.cmake.directory_name = ""

        return config
