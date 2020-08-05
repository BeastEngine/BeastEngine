from unittest.mock import MagicMock, call

import pytest

from src.commandrunners.cmake.cmake_config_files_creator import CMakeConfigFilesCreator
from src.files.file_opener import FileOpener
from src.config.config_names import ConfigNames
from src.commandrunners.cmake.cmake import CMake
from src.commandrunners.command_runner import CommandRunner
from src.config.config_manager import ConfigManager, Config


class CommonTestData:
    PROJECT_DIR = 'project/dir'
    BUILD_DIR = 'build/dir'
    CMAKE_CONFIG_DIR = 'cmake/config'
    CMAKE_CONFIG_DIR_FULL_PATH = f'{PROJECT_DIR}/cmake/config'

    CONFIG_NAMES = [
        ConfigNames.CONFIG_DEBUG,
        ConfigNames.CONFIG_RELEASE,
        ConfigNames.CONFIG_REL_WITH_DEBUG,
        ConfigNames.CONFIG_MIN_SIZE_REL,
    ]

    CONFIG_NAMES_WITH_VERBOSE = [
        [ConfigNames.CONFIG_DEBUG, True],
        [ConfigNames.CONFIG_DEBUG, False],
        [ConfigNames.CONFIG_RELEASE, True],
        [ConfigNames.CONFIG_RELEASE, False],
        [ConfigNames.CONFIG_REL_WITH_DEBUG, True],
        [ConfigNames.CONFIG_REL_WITH_DEBUG, False],
        [ConfigNames.CONFIG_MIN_SIZE_REL, True],
        [ConfigNames.CONFIG_MIN_SIZE_REL, False],
    ]

    def __init__(self):
        self.command_runner_mock = MagicMock(CommandRunner)
        self.command_runner_mock.run_command = MagicMock()

        self.config_manager_mock = MagicMock(ConfigManager)
        type(self.config_manager_mock).config = self.get_empty_config()

        self.file_opener_mock = MagicMock(FileOpener)
        self.file_opener_mock.open = MagicMock()

        self.cmake_config_creator_mock = MagicMock(CMakeConfigFilesCreator)
        self.cmake_config_creator_mock.generate_main_config = MagicMock()
        self.cmake_config_creator_mock.generate_target_config = MagicMock()

        self.sut = CMake(
            self.command_runner_mock,
            self.cmake_config_creator_mock,
            self.config_manager_mock,
            self.PROJECT_DIR,
            self.BUILD_DIR,
        )

    @staticmethod
    def get_empty_config():
        config = Config()
        config.cmake = Config.CMake()
        config.cmake.directory_name = ''

        return config


@pytest.mark.parametrize('expected_verbose', [True, False])
def test_configure_will_run_proper_configuration_command_with_valid_verbose_argument(expected_verbose):
    test_data = CommonTestData()

    expected_command = f'cmake -S . -B {test_data.BUILD_DIR}'
    expected_cwd = test_data.PROJECT_DIR

    test_data.sut.configure(expected_verbose)
    test_data.command_runner_mock.run_command.assert_called_with(expected_command, expected_cwd, expected_verbose)


@pytest.mark.parametrize(
    ['expected_config_name', 'expected_verbose'],
    CommonTestData.CONFIG_NAMES_WITH_VERBOSE
)
def test_build_will_run_proper_build_command_with_given_config_name_and_verbose_argument(expected_config_name, expected_verbose):
    test_data = CommonTestData()

    expected_command = f'cmake --build . --config {expected_config_name.value}'
    expected_cwd = test_data.BUILD_DIR

    test_data.sut.build(expected_config_name, expected_verbose)
    test_data.command_runner_mock.run_command.assert_called_with(expected_command, expected_cwd, expected_verbose)


@pytest.mark.parametrize('expected_verbose', [True, False])
def test_generate_configs_will_generate_all_config_files_through_config_generator_with_valid_verbose_argument(expected_verbose):
    test_data = CommonTestData()
    expected_config = Config()
    expected_config.cmake = Config.CMake()
    expected_config.cmake.directory_name = test_data.CMAKE_CONFIG_DIR
    expected_config.cmake.lib = Config.CMake.Target()
    expected_config.cmake.exe = Config.CMake.Target()
    expected_config.cmake.tests = Config.CMake.Target()
    type(test_data.config_manager_mock).config = expected_config

    expected_main_config_call = call(expected_config.cmake, test_data.CMAKE_CONFIG_DIR_FULL_PATH, expected_verbose)
    expected_lib_target_call = call(expected_config.cmake.lib, test_data.CMAKE_CONFIG_DIR_FULL_PATH, expected_verbose)
    expected_exe_target_call = call(expected_config.cmake.exe, test_data.CMAKE_CONFIG_DIR_FULL_PATH, expected_verbose)
    expected_tests_target_call = call(expected_config.cmake.tests, test_data.CMAKE_CONFIG_DIR_FULL_PATH, expected_verbose)

    sut = CMake(
        test_data.command_runner_mock,
        test_data.cmake_config_creator_mock,
        test_data.config_manager_mock,
        test_data.PROJECT_DIR,
        test_data.BUILD_DIR,
    )
    sut.generate_configs(expected_verbose)

    test_data \
        .cmake_config_creator_mock \
        .generate_main_config \
        .assert_has_calls([expected_main_config_call])

    test_data\
        .cmake_config_creator_mock\
        .generate_target_config\
        .assert_has_calls([expected_lib_target_call, expected_exe_target_call, expected_tests_target_call])


@pytest.mark.parametrize('expected_verbose', [True, False])
def test_generate_main_config_will_generate_config_file_through_config_generator_with_valid_verbose_option_passed(expected_verbose):
    test_data = CommonTestData()
    expected_config = Config()
    expected_config.cmake = Config.CMake()
    expected_config.cmake.directory_name = test_data.CMAKE_CONFIG_DIR
    type(test_data.config_manager_mock).config = expected_config

    sut = CMake(
        test_data.command_runner_mock,
        test_data.cmake_config_creator_mock,
        test_data.config_manager_mock,
        test_data.PROJECT_DIR,
        test_data.BUILD_DIR,
    )
    sut.generate_main_config(expected_verbose)

    test_data\
        .cmake_config_creator_mock\
        .generate_main_config\
        .assert_called_with(expected_config.cmake, test_data.CMAKE_CONFIG_DIR_FULL_PATH, expected_verbose)


@pytest.mark.parametrize('expected_verbose', [True, False])
def test_generate_targets_configs_will_generate_config_files_through_config_generator_with_valid_verbose_argument(expected_verbose):
    test_data = CommonTestData()

    config = Config()
    config.cmake = Config.CMake()
    config.cmake.directory_name = test_data.CMAKE_CONFIG_DIR
    config.cmake.lib = Config.CMake.Target()
    config.cmake.exe = Config.CMake.Target()
    config.cmake.tests = Config.CMake.Target()

    type(test_data.config_manager_mock).config = config

    expected_lib_target_call = call(config.cmake.lib, test_data.CMAKE_CONFIG_DIR_FULL_PATH, expected_verbose)
    expected_exe_target_call = call(config.cmake.exe, test_data.CMAKE_CONFIG_DIR_FULL_PATH, expected_verbose)
    expected_tests_target_call = call(config.cmake.tests, test_data.CMAKE_CONFIG_DIR_FULL_PATH, expected_verbose)

    sut = CMake(
        test_data.command_runner_mock,
        test_data.cmake_config_creator_mock,
        test_data.config_manager_mock,
        test_data.PROJECT_DIR,
        test_data.BUILD_DIR,
    )
    sut.generate_targets_configs(expected_verbose)

    test_data \
        .cmake_config_creator_mock \
        .generate_target_config \
        .assert_has_calls([expected_lib_target_call, expected_exe_target_call, expected_tests_target_call])


@pytest.mark.parametrize('expected_verbose', [True, False])
def test_generate_target_config_will_generate_config_file_through_config_generator_with_valid_verbose_argument(expected_verbose):
    test_data = CommonTestData()

    config = Config()
    config.cmake = Config.CMake()
    config.cmake.directory_name = test_data.CMAKE_CONFIG_DIR
    type(test_data.config_manager_mock).config = config

    expected_target_config = Config.CMake.Target()

    sut = CMake(
        test_data.command_runner_mock,
        test_data.cmake_config_creator_mock,
        test_data.config_manager_mock,
        test_data.PROJECT_DIR,
        test_data.BUILD_DIR,
    )
    sut.generate_target_config(expected_target_config, expected_verbose)

    test_data \
        .cmake_config_creator_mock \
        .generate_target_config \
        .assert_called_with(expected_target_config, test_data.CMAKE_CONFIG_DIR_FULL_PATH, expected_verbose)
