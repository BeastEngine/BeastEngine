import argparse
import builtins
import sys
import pytest

from mock import MagicMock, call

from src.beastengine.beast_command_helper import BeastCommandHelper
from src.beastengine.commands.class_commands.class_files_helper import ClassFilesHelper
from src.config.config_manager import ConfigManager, Config
from src.beastengine.commands.class_commands import class_command
from tests.tests_utilities.micro_mock import MicroMock


class CommonTestData:
    def __init__(self):
        self.parser_mock = MagicMock(argparse.ArgumentParser)
        self.parser_mock.parse_args = MagicMock()
        self.project_dir = 'project/dir'

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


def test_constructor_will_retrieve_only_third_and_fourth_arguments():
    test_data = CommonTestData()
    test_data.mock_create_arguments_parser_function()

    sys.argv = ['arg1', 'arg2', 'arg3', 'arg4', 'arg5']
    expected_arguments = ['arg3', 'arg4']

    class_command.ClassCommand(
        test_data.config_manager_mock,
        test_data.cmake_mock,
        test_data.target_config_manager_mock,

        test_data.class_files_helper_mock
    )
    test_data.parser_mock.parse_args.assert_called_with(expected_arguments)


def test_constructor_will_add_required_command_argument_to_parser():
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
        .assert_has_calls(
            [call(expected_argument_name, help=expected_argument_help, metavar=expected_argument_metavar)],
            any_order=True
        )


def test_constructor_will_add_required_target_argument_to_parser():
    expected_argument_name = 'target'
    expected_argument_help = 'target for which the files should be added'
    expected_argument_metavar = '<target>'

    test_data = CommonTestData()
    test_data.mock_create_arguments_parser_function()
    test_data.parser_mock.add_argument = MagicMock()

    test_data.class_files_helper_mock.class_files_exist = MagicMock(return_value=False)

    class_command.ClassCommand(
        test_data.config_manager_mock,
        test_data.cmake_mock,
        test_data.target_config_manager_mock,
        test_data.class_files_helper_mock
    )
    test_data \
        .parser_mock \
        .add_argument \
        .assert_has_calls(
            [call(expected_argument_name, help=expected_argument_help, metavar=expected_argument_metavar)],
            any_order=True
        )


def test_constructor_will_retrieve_headers_base_directory():
    expected_headers_base_directory = 'headers/base/directory'

    test_data = CommonTestData()
    test_data.mock_create_arguments_parser_function()
    test_data.config_manager_mock.get_target_config_by_name = MagicMock(return_value=test_data.config.cmake.lib)
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
    test_data.config_manager_mock.get_target_config_by_name = MagicMock(return_value=test_data.config.cmake.lib)
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


@pytest.mark.parametrize('expected_verbose', [True, False])
def test_constructor_will_call_add_class_command_when_valid_command_line_argument_is_passed(expected_verbose):
    headers_base_directory = 'headers/base/directory'
    sources_base_directory = 'sources/base/directory'
    expected_command = BeastCommandHelper.COMMAND_NAME_CLASS_ADD

    test_data = CommonTestData()
    test_data.mock_create_arguments_parser_function()

    cli_arguments_mock = MicroMock(command=expected_command, target=ConfigManager.TARGET_NAME_LIB)
    test_data.parser_mock.parse_args = MagicMock(return_value=cli_arguments_mock)
    test_data.config_manager_mock.get_target_config_by_name = MagicMock(return_value=test_data.config.cmake.lib)
    test_data.target_config_manager_mock.get_headers_base_directory = MagicMock(return_value=headers_base_directory)
    test_data.target_config_manager_mock.get_sources_base_directory = MagicMock(return_value=sources_base_directory)

    class_command.ClassAdd = MagicMock()
    class_command.ClassRemove = MagicMock()
    class_command.is_verbose_set = MagicMock(return_value=expected_verbose)

    class_command.ClassCommand(
        test_data.config_manager_mock,
        test_data.cmake_mock,
        test_data.target_config_manager_mock,
        test_data.class_files_helper_mock
    )

    class_command.ClassAdd.assert_called_with(
        headers_base_directory,
        sources_base_directory,
        test_data.class_files_helper_mock,
        test_data.config.cmake.lib,
        test_data.config_manager_mock
    )
    class_command.ClassRemove.assert_not_called()

    test_data.cmake_mock.generate_configs.assert_called_with(expected_verbose)


@pytest.mark.parametrize('expected_verbose', [True, False])
def test_constructor_will_call_remove_class_command_when_valid_command_line_argument_is_passed(expected_verbose):
    headers_base_directory = 'headers/base/directory'
    sources_base_directory = 'sources/base/directory'
    expected_command = BeastCommandHelper.COMMAND_NAME_CLASS_REMOVE

    test_data = CommonTestData()
    test_data.mock_create_arguments_parser_function()

    cli_arguments_mock = MicroMock(command=expected_command, target=ConfigManager.TARGET_NAME_LIB)
    test_data.parser_mock.parse_args = MagicMock(return_value=cli_arguments_mock)
    test_data.config_manager_mock.get_target_config_by_name = MagicMock(return_value=test_data.config.cmake.lib)
    test_data.target_config_manager_mock.get_headers_base_directory = MagicMock(return_value=headers_base_directory)
    test_data.target_config_manager_mock.get_sources_base_directory = MagicMock(return_value=sources_base_directory)

    class_command.ClassAdd = MagicMock()
    class_command.ClassRemove = MagicMock()
    class_command.is_verbose_set = MagicMock(return_value=expected_verbose)

    class_command.ClassCommand(
        test_data.config_manager_mock,
        test_data.cmake_mock,
        test_data.target_config_manager_mock,
        test_data.class_files_helper_mock
    )

    class_command.ClassAdd.assert_not_called()
    class_command.ClassRemove.assert_called_with(
        headers_base_directory,
        sources_base_directory,
        test_data.class_files_helper_mock,
        test_data.config.cmake.lib,
        test_data.config_manager_mock
    )
    test_data.cmake_mock.generate_configs.assert_called_with(expected_verbose)


@pytest.mark.parametrize('expected_verbose', [True, False])
def test_constructor_will_show_headers_and_sources_base_directories_when_valid_command_line_argument_is_passed(expected_verbose):
    headers_base_directory = 'headers/base/directory'
    sources_base_directory = 'sources/base/directory'
    expected_command = BeastCommandHelper.COMMAND_NAME_CLASS_PATH_SHOW

    test_data = CommonTestData()
    test_data.mock_create_arguments_parser_function()
    test_data.mock_print_function()

    cli_arguments_mock = MicroMock(command=expected_command, target=ConfigManager.TARGET_NAME_LIB)
    test_data.parser_mock.parse_args = MagicMock(return_value=cli_arguments_mock)
    test_data.config_manager_mock.get_target_config_by_name = MagicMock(return_value=test_data.config.cmake.lib)
    test_data.target_config_manager_mock.get_headers_base_directory = MagicMock(return_value=headers_base_directory)
    test_data.target_config_manager_mock.get_sources_base_directory = MagicMock(return_value=sources_base_directory)

    class_command.ClassAdd = MagicMock()
    class_command.ClassRemove = MagicMock()
    class_command.is_verbose_set = MagicMock(return_value=expected_verbose)

    class_command.ClassCommand(
        test_data.config_manager_mock,
        test_data.cmake_mock,
        test_data.target_config_manager_mock,
        test_data.class_files_helper_mock
    )

    class_command.ClassAdd.assert_not_called()
    class_command.ClassRemove.assert_not_called()
    test_data.cmake_mock.generate_configs.assert_not_called()

    test_data.print_mock.assert_called_with(
        f'Headers base directory: {headers_base_directory}\nSources base directory: {sources_base_directory}'
    )


def test_constructor_will_print_error_message_when_invalid_target_defined():
    invalid_target_name = 'invalid'
    expected_error_message = f'\'{invalid_target_name}\' is not a valid target!'

    test_data = CommonTestData()
    test_data.mock_create_arguments_parser_function()
    cli_arguments_mock = MicroMock(command='command', target=invalid_target_name)
    test_data.parser_mock.parse_args = MagicMock(return_value=cli_arguments_mock)
    test_data.config_manager_mock.get_target_config_by_name = MagicMock(return_value=None)

    test_data.mock_print_function()

    class_command.ClassCommand(
        test_data.config_manager_mock,
        test_data.cmake_mock,
        test_data.target_config_manager_mock,
        test_data.class_files_helper_mock
    )

    test_data.print_mock.assert_called_with(expected_error_message)
