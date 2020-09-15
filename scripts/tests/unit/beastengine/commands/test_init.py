import argparse
import sys

import pytest

import src.beastengine.commands.init as init

from mock import MagicMock, call

from src.commandrunners.cmake.cmake import CMake
from src.commandrunners.conan import Conan
from src.commandrunners.command_runner import CommandRunner


class CommonTestData:
    def __init__(self):
        self.argv = ['arg1', 'arg2', 'arg3', 'arg4']

        self.parser_mock = MagicMock(argparse.ArgumentParser)
        self.parser_mock.parse_args = MagicMock()
        self.project_dir = 'project/dir'

        init.create_arguments_parser = MagicMock(return_value=self.parser_mock)

    def set_sys_argv(self):
        sys.argv = self.argv

    def mock_is_verbose_set_function_to_return(self, return_value):
        init.is_verbose_set = MagicMock(return_value=return_value)

    def mock_get_build_dir_name_function_to_return(self, return_value):
        init.get_build_dir_name = MagicMock(return_value=return_value)


def test_constructor_will_retrieve_all_arguments_starting_from_third():
    test_data = CommonTestData()

    sys.argv = ['arg1', 'arg2', 'arg3', 'arg4']
    expected_arguments = ['arg3', 'arg4']

    init.Init('project_dir', MagicMock(), MagicMock(), MagicMock())
    test_data.parser_mock.parse_args.assert_called_with(expected_arguments)


def test_constructor_will_recreate_build_directory():
    test_data = CommonTestData()
    test_data.set_sys_argv()

    expected_project_dir = 'project/dir'
    expected_build_dir_name = 'expected_build_dir_name'
    expected_command_remove = f'rm -rf {expected_build_dir_name}'
    expected_command_create = f'mkdir {expected_build_dir_name}'
    is_verbose_set = False

    test_data.mock_get_build_dir_name_function_to_return(expected_build_dir_name)
    test_data.mock_is_verbose_set_function_to_return(is_verbose_set)

    command_runner_mock = MagicMock(CommandRunner)
    command_runner_mock.run_command = MagicMock()
    expected_calls = [
        call(expected_command_remove, expected_project_dir, is_verbose_set),
        call(expected_command_create, expected_project_dir, is_verbose_set)
    ]

    init.Init(expected_project_dir, command_runner_mock, MagicMock(), MagicMock())
    command_runner_mock.run_command.assert_has_calls(expected_calls)


def test_constructor_will_run_conan_install():
    test_data = CommonTestData()
    test_data.set_sys_argv()

    conan_mock = MagicMock(Conan)
    conan_mock.install = MagicMock()

    init.Init(test_data.project_dir, MagicMock(), conan_mock, MagicMock())
    conan_mock.install.assert_called_once()


def test_constructor_will_generate_cmake_configs():
    test_data = CommonTestData()
    test_data.set_sys_argv()

    cmake_mock = MagicMock(CMake)
    cmake_mock.generate_configs = MagicMock()

    init.Init(test_data.project_dir, MagicMock(), MagicMock(), cmake_mock)
    cmake_mock.generate_configs.assert_called_once()


@pytest.mark.parametrize('expected_is_verbose_set', [True, False])
def test_constructor_will_execute_command_with_verbose_option_with_valid_verbose_option(expected_is_verbose_set):
    test_data = CommonTestData()

    project_dir = 'project/dir'
    build_dir_name = 'build_dir_name'
    command_remove = f'rm -rf {build_dir_name}'
    command_create = f'mkdir {build_dir_name}'

    test_data.mock_get_build_dir_name_function_to_return(build_dir_name)
    init.is_verbose_set = MagicMock(return_value=expected_is_verbose_set)

    command_runner_mock = MagicMock(CommandRunner)
    command_runner_mock.run_command = MagicMock()
    expected_calls = [
        call(command_remove, project_dir, expected_is_verbose_set),
        call(command_create, project_dir, expected_is_verbose_set)
    ]

    init.Init(project_dir, command_runner_mock, MagicMock(), MagicMock())

    init.is_verbose_set.assert_called_once()
    command_runner_mock.run_command.assert_has_calls(expected_calls)


@pytest.mark.parametrize('expected_is_verbose_set', [True, False])
def test_constructor_will_execute_conan_install_command_with_valid_verbose_option(expected_is_verbose_set):
    test_data = CommonTestData()
    sys.argv = ['arg1', 'arg2', '-v']

    project_dir = 'project/dir'
    build_dir_name = 'build_dir_name'

    test_data.mock_get_build_dir_name_function_to_return(build_dir_name)
    init.is_verbose_set = MagicMock(return_value=expected_is_verbose_set)

    conan_mock = MagicMock(Conan)
    conan_mock.install = MagicMock()

    init.Init(project_dir, MagicMock(), conan_mock, MagicMock())
    conan_mock.install.assert_called_once_with(expected_is_verbose_set)


@pytest.mark.parametrize('expected_is_verbose_set', [True, False])
def test_constructor_will_execute_cmake_install_command_with_valid_verbose_option(expected_is_verbose_set):
    test_data = CommonTestData()

    project_dir = 'project/dir'
    build_dir_name = 'build_dir_name'

    test_data.mock_get_build_dir_name_function_to_return(build_dir_name)
    init.is_verbose_set = MagicMock(return_value=expected_is_verbose_set)

    cmake_mock = MagicMock(CMake)
    cmake_mock.generate_configs = MagicMock()

    init.Init(project_dir, MagicMock(), MagicMock(), cmake_mock)
    cmake_mock.generate_configs.assert_called_once_with(expected_is_verbose_set)
