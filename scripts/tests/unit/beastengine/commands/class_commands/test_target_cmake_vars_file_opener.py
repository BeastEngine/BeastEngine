from copy import deepcopy

import pytest
from mock import MagicMock

from src.files.file_opener import FileOpener
from src.config.config_manager import Config
import src.beastengine.commands.class_commands.target_cmake_vars_file_opener as file_opener


class CommonTestData:
    def __init__(self):
        self.config = Config()
        self.config.cmake = Config.CMake()
        self.config.cmake.directory_name = 'dir_name'

        self.config.cmake.lib = Config.CMake.Target()
        self.config.cmake.lib.variables = Config.CMake.Target.Variables()
        self.config.cmake.lib.variables.target_cmake_variables_file_path = 'file_path'

        self.file_mock = MagicMock(FileOpener.File)
        self.file_opener_mock = MagicMock(FileOpener)
        self.file_opener_mock.open = MagicMock(return_value=self.file_mock)


def test_open_will_get_full_file_path():
    expected_dir_name = 'dir_name'
    expected_file_path = 'target/cmake/vars/file/path.txt'

    test_data = CommonTestData()
    config = deepcopy(test_data.config.cmake)
    config.directory_name = expected_dir_name
    config.lib.variables.target_cmake_variables_file_path = expected_file_path

    file_opener.get_target_cmake_variables_full_file_path = MagicMock()

    sut = file_opener.TargetCMakeVarsFileOpener(test_data.file_opener_mock)
    sut.open(config, config.lib)

    file_opener.get_target_cmake_variables_full_file_path.assert_called_with(expected_dir_name, config.lib.variables)


def test_open_will_open_proper_file():
    expected_dir_name = 'dir_name'
    expected_file_path = 'target/cmake/vars/file/path.txt'
    expected_full_file_path = f'{expected_dir_name}/{expected_file_path}'

    test_data = CommonTestData()
    config = deepcopy(test_data.config.cmake)
    config.directory_name = expected_dir_name
    config.lib.variables.target_cmake_variables_file_path = expected_file_path

    file_opener.get_target_cmake_variables_full_file_path = MagicMock(return_value=expected_full_file_path)

    sut = file_opener.TargetCMakeVarsFileOpener(test_data.file_opener_mock)
    sut.open(config, config.lib)

    test_data.file_opener_mock.open.assert_called_with(expected_full_file_path)


def test_open_will_return_file_content_arranged_in_map_of_variables_and_values():
    full_file_path = 'expected_file_path'
    variable1_name = 'var_1'
    variable1_value = 'var_1_value'

    variable2_name = 'var_2'
    variable2_value = 'var_2_value'

    file_content_before_split = f'''{variable1_name}={variable1_value}\n{variable2_name}={variable2_value}'''
    expected_content_map = {variable1_name: variable1_value, variable2_name: variable2_value}

    test_data = CommonTestData()
    test_data.file_mock.get_content = MagicMock(return_value=file_content_before_split)
    file_opener.get_target_cmake_variables_full_file_path = MagicMock(return_value=full_file_path)

    sut = file_opener.TargetCMakeVarsFileOpener(test_data.file_opener_mock)
    actual_content_map = sut.open(test_data.config.cmake, test_data.config.cmake.lib)

    assert actual_content_map == expected_content_map


def test_open_will_throw_exception_if_any_line_in_file_does_not_contain_variable():
    with pytest.raises(ValueError):
        full_file_path = 'expected_file_path'
        variable1_name = 'var_1'
        variable1_value = 'var_1_value'

        variable2_name = 'var_2'
        variable2_value = 'var_2_value'

        file_content_before_split = f'''{variable1_name}{variable1_value}\n{variable2_name}={variable2_value}'''

        test_data = CommonTestData()
        test_data.file_mock.get_content = MagicMock(return_value=file_content_before_split)
        file_opener.get_target_cmake_variables_full_file_path = MagicMock(return_value=full_file_path)

        sut = file_opener.TargetCMakeVarsFileOpener(test_data.file_opener_mock)
        sut.open(test_data.config.cmake, test_data.config.cmake.lib)
