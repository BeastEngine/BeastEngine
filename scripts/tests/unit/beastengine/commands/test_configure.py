import argparse
import sys

import pytest
from mock import MagicMock

from src.commandrunners.cmake.cmake import CMake
from src.beastengine.commands import configure


class CommonTestData:
    def __init__(self):
        self.argv = ['arg1', 'arg2', 'arg3', 'arg4']

        self.parser_mock = MagicMock(argparse.ArgumentParser)
        self.parser_mock.parse_args = MagicMock()
        self.project_dir = "project/dir"

    def mock_create_arguments_parser_function(self):
        configure.create_arguments_parser = MagicMock(return_value=self.parser_mock)

    def mock_is_verbose_set_function_to_return(self, return_value):
        configure.is_verbose_set = MagicMock(return_value=return_value)

    def mock_get_build_dir_name_function_to_return(self, return_value):
        configure.get_build_dir_name = MagicMock(return_value=return_value)


def test_constructor_will_retrieve_all_arguments_starting_from_third():
    test_data = CommonTestData()
    test_data.mock_create_arguments_parser_function()

    sys.argv = ['arg1', 'arg2', 'arg3', 'arg4']
    expected_arguments = ['arg3', 'arg4']

    configure.Configure(MagicMock(CMake))
    test_data.parser_mock.parse_args.assert_called_with(expected_arguments)


def test_constructor_will_check_if_verbose_option_is_set():
    test_data = CommonTestData()
    test_data.mock_create_arguments_parser_function()

    configure.is_verbose_set = MagicMock(return_value=True)

    configure.Configure(MagicMock(CMake))
    configure.is_verbose_set.assert_called_once()


@pytest.mark.parametrize('expected_verbose', [True, False])
def test_constructor_will_run_cmake_configure_command_with_valid_verbose_argument(expected_verbose):
    test_data = CommonTestData()
    test_data.mock_create_arguments_parser_function()

    build_dir_name = "build_dir_name"

    test_data.mock_get_build_dir_name_function_to_return(build_dir_name)
    test_data.mock_is_verbose_set_function_to_return(expected_verbose)

    cmake_mock = MagicMock(CMake)
    cmake_mock.generate_configs = MagicMock()

    configure.Configure(cmake_mock)
    cmake_mock.configure.assert_called_once_with(expected_verbose)
