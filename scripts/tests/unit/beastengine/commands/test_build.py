import argparse
import builtins
import sys
import colorama
import pytest

from mock import MagicMock

from src.commandrunners.cmake.cmake import CMake
from src.config.config_manager import ConfigManager, Config
from src.beastengine.commands import build
from tests.tests_utilities.micro_mock import MicroMock


class CommonTestData:
    CONFIG_NAMES = [
        build.ConfigNames.CONFIG_DEBUG,
        build.ConfigNames.CONFIG_RELEASE,
        build.ConfigNames.CONFIG_MIN_SIZE_REL,
        build.ConfigNames.CONFIG_REL_WITH_DEBUG,
    ]

    def __init__(self):
        self.parser_mock = MagicMock(argparse.ArgumentParser)
        self.parser_mock.parse_args = MagicMock()
        self.project_dir = 'project/dir'

        self.config = Config()
        self.config_manager_mock = MagicMock(ConfigManager)
        self.config_manager_mock.config = self.config

        self.print_mock = MagicMock()
        self.config_mock = MagicMock(build.ConfigNames)

        self.original_print = builtins.print

    def mock_create_arguments_parser_function(self):
        build.create_arguments_parser = MagicMock(return_value=self.parser_mock)

    def mock_is_verbose_set_function_to_return(self, return_value):
        build.is_verbose_set = MagicMock(return_value=return_value)

    def mock_get_build_dir_name_function_to_return(self, return_value):
        build.get_build_dir_name = MagicMock(return_value=return_value)

    def mock_print_function(self):
        builtins.print = self.print_mock

    def mock_config_names_from_string_method(self, side_effect=None):
        build.ConfigNames.from_string = MagicMock(return_value=self.config_mock, side_effect=side_effect)


def test_constructor_will_retrieve_all_arguments_starting_from_third():
    test_data = CommonTestData()
    test_data.mock_create_arguments_parser_function()

    sys.argv = ['arg1', 'arg2', 'arg3', 'arg4']
    expected_arguments = ['arg3', 'arg4']

    build.Build(test_data.config_manager_mock, MagicMock(CMake))
    test_data.parser_mock.parse_args.assert_called_with(expected_arguments)


def test_constructor_will_check_if_verbose_option_is_set():
    test_data = CommonTestData()
    test_data.mock_create_arguments_parser_function()

    build.is_verbose_set = MagicMock(return_value=True)

    build.Build(test_data.config_manager_mock, MagicMock(CMake))
    build.is_verbose_set.assert_called_once()


def test_constructor_will_save_cl_arguments_into_variable():
    sys.argv = ['arg1', 'arg2', 'arg3', 'arg4']
    expected_arguments = ['arg3', 'arg4']

    test_data = CommonTestData()
    test_data.parser_mock.parse_args = MagicMock(return_value=expected_arguments)
    test_data.mock_create_arguments_parser_function()

    sut = build.Build(test_data.config_manager_mock, MagicMock(CMake))
    assert sut.command_line_arguments == expected_arguments


@pytest.mark.parametrize('expected_verbose', [True, False])
def test_constructor_will_save_verbose_argument_into_variable(expected_verbose):
    test_data = CommonTestData()
    test_data.mock_create_arguments_parser_function()

    build.is_verbose_set = MagicMock(return_value=expected_verbose)

    sut = build.Build(test_data.config_manager_mock, MagicMock(CMake))
    assert sut.is_verbose == expected_verbose


@pytest.mark.parametrize('config', ['', False, None])
def test_execute_will_print_warning_message_when_passed_arguments_config_parameter_is_empty(config):
    expected_arguments = MicroMock(config=config)
    expected_info_message =\
        f'{colorama.Fore.YELLOW}' \
        f'No configuration specified, ' \
        f'building for all configurations {colorama.Fore.RESET}'

    test_data = CommonTestData()
    test_data.mock_print_function()
    test_data.mock_config_names_from_string_method()

    test_data.parser_mock.parse_args = MagicMock(return_value=expected_arguments)
    test_data.mock_create_arguments_parser_function()

    sut = build.Build(test_data.config_manager_mock, MagicMock(CMake))
    sut.execute()

    test_data.print_mock.assert_called_with(expected_info_message)

@pytest.mark.parametrize(
    'expected_config', CommonTestData.CONFIG_NAMES
)
def test_execute_will_convert_config_string_to_enum(expected_config):
    expected_arguments = MicroMock(config=expected_config)

    test_data = CommonTestData()
    test_data.mock_print_function()
    test_data.mock_config_names_from_string_method()

    test_data.parser_mock.parse_args = MagicMock(return_value=expected_arguments)
    test_data.mock_create_arguments_parser_function()

    sut = build.Build(test_data.config_manager_mock, MagicMock(CMake))
    sut.execute()

    build.ConfigNames.from_string.assert_called_once_with(expected_config)


def test_execute_will_print_error_message_when_passed_configuration_is_not_valid():
    invalid_config = 'invalid_config'
    expected_arguments = MicroMock(config=invalid_config)

    expected_default_config = build.ConfigNames.CONFIG_DEBUG
    expected_info_message =\
        f"{colorama.Fore.LIGHTRED_EX}'{invalid_config}' is not a valid configuration!\n" \
        f"{colorama.Fore.YELLOW}The available configurations are: {build.ConfigNames.available_names()}"

    test_data = CommonTestData()
    test_data.mock_print_function()
    test_data.config.default_build_type = expected_default_config

    test_data.parser_mock.parse_args = MagicMock(return_value=expected_arguments)
    test_data.mock_create_arguments_parser_function()

    test_data.mock_config_names_from_string_method(side_effect=ValueError)

    sut = build.Build(test_data.config_manager_mock, MagicMock(CMake))
    sut.execute()

    test_data.print_mock.assert_called_with(expected_info_message)


@pytest.mark.parametrize(
    'expected_config', CommonTestData.CONFIG_NAMES
)
def test_execute_will_run_cmake_build_command_with_proper_config_name(expected_config):
    arguments = MicroMock(config=expected_config)

    test_data = CommonTestData()
    test_data.mock_config_names_from_string_method()
    test_data.mock_is_verbose_set_function_to_return(False)

    expected_config = test_data.config_mock
    config_argument_in_method_call_index = 0

    test_data.parser_mock.parse_args = MagicMock(return_value=arguments)
    test_data.mock_create_arguments_parser_function()

    cmake_mock = MagicMock(CMake)
    cmake_mock.build = MagicMock()

    sut = build.Build(test_data.config_manager_mock, cmake_mock)
    sut.execute()

    method_call = cmake_mock.build.call_args[0]
    assert method_call[config_argument_in_method_call_index] == expected_config


@pytest.mark.parametrize('expected_verbose', [True, False])
def test_execute_will_run_cmake_build_command_with_valid_verbose_argument(expected_verbose):
    test_data = CommonTestData()
    test_data.mock_config_names_from_string_method()
    test_data.mock_create_arguments_parser_function()
    test_data.mock_is_verbose_set_function_to_return(False)

    verbose_argument_in_method_call_index = 1
    build.is_verbose_set = MagicMock(return_value=expected_verbose)

    cmake_mock = MagicMock(CMake)
    cmake_mock.build = MagicMock()

    sut = build.Build(test_data.config_manager_mock, cmake_mock)
    sut.execute()

    method_call = cmake_mock.build.call_args[0]
    assert method_call[verbose_argument_in_method_call_index] == expected_verbose


def test_execute_will_not_run_cmake_build_command_when_invalid_config_name_passed():
    invalid_config = 'invalid_config'
    expected_arguments = MicroMock(config=invalid_config)

    test_data = CommonTestData()
    test_data.mock_print_function()

    test_data.parser_mock.parse_args = MagicMock(return_value=expected_arguments)
    test_data.mock_create_arguments_parser_function()
    test_data.mock_config_names_from_string_method(side_effect=ValueError)

    cmake_mock = MagicMock(CMake)
    cmake_mock.build = MagicMock()

    sut = build.Build(test_data.config_manager_mock, cmake_mock)
    sut.execute()

    cmake_mock.build.assert_not_called()
