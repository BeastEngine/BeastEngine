from unittest.mock import call, MagicMock

import pytest

from src.commandrunners.command_runner import CommandRunner
from src.commandrunners.conan import Conan
from src.config.config_names import ConfigNames


def test_install_will_execute_proper_command_with_each_config_name():
    command_runner_mock = MagicMock(CommandRunner)
    command_runner_mock.run_command = MagicMock()
    verbose = False

    expected_build_dir = "build/dir"

    expected_debug_command = f"conan install .. -g cmake_multi --build=missing -s build_type={ConfigNames.CONFIG_DEBUG}"
    expected_release_command = f"conan install .. -g cmake_multi --build=missing -s build_type={ConfigNames.CONFIG_RELEASE}"
    expected_rel_with_debug_command = f"conan install .. -g cmake_multi --build=missing -s build_type={ConfigNames.CONFIG_REL_WITH_DEBUG}"
    expected_min_size_rel_command = f"conan install .. -g cmake_multi --build=missing -s build_type={ConfigNames.CONFIG_MIN_SIZE_REL}"

    expected_debug_call = call(expected_debug_command, expected_build_dir, verbose)
    expected_release_call = call(expected_release_command, expected_build_dir, verbose)
    expected_rel_with_debug_call = call(expected_rel_with_debug_command, expected_build_dir, verbose)
    expected_min_size_rel_call = call(expected_min_size_rel_command, expected_build_dir, verbose)

    sut = Conan(command_runner_mock, expected_build_dir)
    sut.install(verbose)

    command_runner_mock\
        .run_command\
        .assert_has_calls(
            [
                expected_debug_call,
                expected_release_call,
                expected_rel_with_debug_call,
                expected_min_size_rel_call
            ]
        )


@pytest.mark.parametrize('expected_verbose', [False, True])
def test_install_will_pass_verbose_argument_to_command_runner(expected_verbose):
    command_runner_mock = MagicMock(CommandRunner)
    command_runner_mock.run_command = MagicMock()
    build_dir = "build/dir"

    verbose_argument_in_method_call_index = 2

    sut = Conan(command_runner_mock, build_dir)
    sut.install(expected_verbose)

    for single_call in command_runner_mock.run_command.call_args_list:
        single_call_arguments_list = single_call[0]
        assert single_call_arguments_list[verbose_argument_in_method_call_index] == expected_verbose
