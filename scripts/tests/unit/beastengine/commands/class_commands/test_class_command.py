import argparse
import builtins
import sys
import pytest

from mock import MagicMock

from src.beastengine.commands.class_commands.class_files_helper import ClassFilesHelper
from src.config.config_manager import ConfigManager, Config
from src.beastengine.commands.class_commands import class_command
from tests.tests_utilities.micro_mock import MicroMock


class CommonTestData:
    def __init__(self):
        self.argv = ['arg1', 'arg2', 'arg3', 'arg4']

        self.parser_mock = MagicMock(argparse.ArgumentParser)
        self.parser_mock.parse_args = MagicMock()
        self.project_dir = "project/dir"

        self.config = Config()
        self.config.cmake = Config.CMake()
        self.config.cmake.lib = Config.CMake.Target()

        self.config_manager_mock = MagicMock(ConfigManager)
        self.config_manager_mock.config = self.config

        self.print_mock = MagicMock()
        self.class_files_helper_mock = MagicMock(ClassFilesHelper)

        self.cmake_mock = MagicMock(class_command.CMake)
        self.target_config_manager_mock = MagicMock(class_command.TargetConfigManager)

        self.original_print = builtins.print

        class_command.ClassAdd = MagicMock()

    def mock_create_arguments_parser_function(self):
        class_command.create_arguments_parser = MagicMock(return_value=self.parser_mock)

    def mock_is_verbose_set_function_to_return(self, return_value):
        class_command.is_verbose_set = MagicMock(return_value=return_value)

    def mock_print_function(self):
        builtins.print = self.print_mock


def test_constructor_will_retrieve_only_third_argument():
    test_data = CommonTestData()
    test_data.mock_create_arguments_parser_function()

    sys.argv = ['arg1', 'arg2', 'arg3', 'arg4']
    expected_arguments = ['arg3']

    class_command.ClassCommand(
        test_data.config_manager_mock,
        test_data.cmake_mock,
        test_data.target_config_manager_mock,

        test_data.class_files_helper_mock
    )
    test_data.parser_mock.parse_args.assert_called_with(expected_arguments)


def test_constructor_will_add_required_argument_to_parser():
    expected_argument_name = 'command'
    expected_argument_help = 'command to execute'
    expected_argument_metavar = '<command>'

    test_data = CommonTestData()
    test_data.mock_create_arguments_parser_function()
    test_data.parser_mock.add_argument = MagicMock()

    class_command.ClassCommand(
        test_data.config_manager_mock,
        test_data.cmake_mock,
        test_data.target_config_manager_mock,
        test_data.class_files_helper_mock
    )

    test_data\
        .parser_mock\
        .add_argument\
        .assert_called_with(expected_argument_name, help=expected_argument_help, metavar=expected_argument_metavar)


def test_constructor_will_retrieve_headers_base_directory():
    expected_headers_base_directory = 'headers/base/directory'

    test_data = CommonTestData()
    test_data.mock_create_arguments_parser_function()
    test_data.target_config_manager_mock.get_headers_base_directory = MagicMock(return_value=expected_headers_base_directory)

    class_command.ClassCommand(
        test_data.config_manager_mock,
        test_data.cmake_mock,
        test_data.target_config_manager_mock,
        test_data.class_files_helper_mock
    )

    test_data\
        .target_config_manager_mock\
        .get_headers_base_directory\
        .assert_called_with(test_data.config.cmake.lib, test_data.config.cmake)


def test_constructor_will_retrieve_sources_base_directory():
    expected_sources_base_directory = 'sources/base/directory'

    test_data = CommonTestData()
    test_data.mock_create_arguments_parser_function()
    test_data.target_config_manager_mock.get_sources_base_directory = MagicMock(return_value=expected_sources_base_directory)

    class_command.ClassCommand(
        test_data.config_manager_mock,
        test_data.cmake_mock,
        test_data.target_config_manager_mock,
        test_data.class_files_helper_mock
    )

    test_data\
        .target_config_manager_mock\
        .get_sources_base_directory\
        .assert_called_with(test_data.config.cmake.lib, test_data.config.cmake)


def test_constructor_will_call_add_class_command_when_valid_command_line_argument_is_passed():
    expected_headers_base_directory = 'headers/base/directory'
    expected_sources_base_directory = 'sources/base/directory'
    expected_command = 'add'

    test_data = CommonTestData()
    test_data.mock_create_arguments_parser_function()

    cli_arguments_mock = MicroMock(command=expected_command)
    test_data.parser_mock.parse_args = MagicMock(return_value=cli_arguments_mock)
    test_data.target_config_manager_mock.get_headers_base_directory = MagicMock(return_value=expected_headers_base_directory)
    test_data.target_config_manager_mock.get_sources_base_directory = MagicMock(return_value=expected_sources_base_directory)

    class_command.ClassAdd = MagicMock()
    class_command.ClassCommand(
        test_data.config_manager_mock,
        test_data.cmake_mock,
        test_data.target_config_manager_mock,
        test_data.class_files_helper_mock
    )

    class_command.ClassAdd.assert_called_with(
        expected_headers_base_directory,
        expected_sources_base_directory,
        test_data.class_files_helper_mock,
        test_data.config.cmake.lib,
        test_data.config_manager_mock
    )
