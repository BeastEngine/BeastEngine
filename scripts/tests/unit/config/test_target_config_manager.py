import pytest
from mock import MagicMock

from src.config.target_config_manager import TargetConfigManager
from src.config.config import Config
from src.beastengine.commands.class_commands.target_cmake_vars_file_opener import TargetCMakeVarsFileOpener


def test_get_headers_base_directory_will_return_empty_string_when_no_base_directory_defined():
    expected_base_directory_path = ''

    cmake_vars_file_opener_mock = MagicMock(TargetCMakeVarsFileOpener)
    cmake_vars_file_opener_mock.open = MagicMock(return_value={})

    config = Config()
    config.cmake = Config.CMake()

    target_config = Config.CMake.Target()
    target_config.headers = Config.CMake.Target.Files
    target_config.headers.base_dir = ''

    sut = TargetConfigManager(cmake_vars_file_opener_mock)
    actual_base_directory_path = sut.get_headers_base_directory(target_config, config.cmake)

    assert actual_base_directory_path == expected_base_directory_path


def test_get_headers_base_directory_will_return_full_headers_base_directory_path_based_on_base_directory():
    expected_base_directory_path = 'base/dir/path'

    cmake_vars_file_opener_mock = MagicMock(TargetCMakeVarsFileOpener)
    cmake_vars_file_opener_mock.open = MagicMock(return_value={})

    config = Config()
    config.cmake = Config.CMake()
    config.cmake.directory_name = 'cmake_dir_name'

    target_config = Config.CMake.Target()
    target_config.headers = Config.CMake.Target.Files
    target_config.headers.base_dir = expected_base_directory_path

    sut = TargetConfigManager(cmake_vars_file_opener_mock)
    actual_base_directory_path = sut.get_headers_base_directory(target_config, config.cmake)

    assert actual_base_directory_path == expected_base_directory_path


def test_get_headers_base_directory_will_return_full_path_when_base_dir_contains_cmake_variable():
    base_dir_name = 'base_dir'

    cmake_variable_name = '${BEAST_INCLUDE_DIR}'
    cmake_variable_value = 'variable_value'
    cmake_variables_map = {cmake_variable_name: cmake_variable_value}

    cmake_vars_file_opener_mock = MagicMock(TargetCMakeVarsFileOpener)
    cmake_vars_file_opener_mock.open = MagicMock(return_value=cmake_variables_map)

    config = Config()
    config.cmake = Config.CMake()
    config.cmake.directory_name = 'cmake_dir_name'

    target_config = Config.CMake.Target()
    target_config.headers = Config.CMake.Target.Files
    target_config.headers.base_dir = f'{cmake_variable_name}/{base_dir_name}'

    expected_base_directory_path = f'{cmake_variable_value}/{base_dir_name}'

    sut = TargetConfigManager(cmake_vars_file_opener_mock)
    actual_base_directory_path = sut.get_headers_base_directory(target_config, config.cmake)

    assert actual_base_directory_path == expected_base_directory_path


def test_get_headers_base_directory_will_return_full_path_when_base_dir_contains_multiple_cmake_variables():
    base_dir_name = 'base_dir'

    cmake_variable1_name = '${BEAST_INCLUDE_DIR}'
    cmake_variable1_value = 'include_dir'
    cmake_variable2_name = '${BEAST_SOME_VARIABLE}'
    cmake_variable2_value = 'some_variable'

    cmake_variables_map = {cmake_variable1_name: cmake_variable1_value, cmake_variable2_name: cmake_variable2_value}

    cmake_vars_file_opener_mock = MagicMock(TargetCMakeVarsFileOpener)
    cmake_vars_file_opener_mock.open = MagicMock(return_value=cmake_variables_map)

    config = Config()
    config.cmake = Config.CMake()
    config.cmake.directory_name = 'cmake_dir_name'

    target_config = Config.CMake.Target()
    target_config.headers = Config.CMake.Target.Files
    target_config.headers.base_dir = f'{cmake_variable1_name}/{cmake_variable2_name}/{base_dir_name}'

    expected_base_directory_path = f'{cmake_variable1_value}/{cmake_variable2_value}/{base_dir_name}'

    sut = TargetConfigManager(cmake_vars_file_opener_mock)
    actual_base_directory_path = sut.get_headers_base_directory(target_config, config.cmake)

    assert actual_base_directory_path == expected_base_directory_path


def test_get_headers_base_directory_will_throw_exception_when_cmake_variable_from_base_dir_does_not_exist_in_vars_file():
    with pytest.raises(ValueError):
        base_dir_name = 'base_dir'

        cmake_variable1_name = '${BEAST_INCLUDE_DIR}'
        cmake_variable1_value = 'include_dir'
        cmake_variable2_name = '${BEAST_SOME_VARIABLE}'
        cmake_variable2_value = 'some_variable'
        non_existent_cmake_variable_name = '${THIS_VARIABLE_DOES_NOT_EXIST}'

        cmake_variables_map = {cmake_variable1_name: cmake_variable1_value, cmake_variable2_name: cmake_variable2_value}

        cmake_vars_file_opener_mock = MagicMock(TargetCMakeVarsFileOpener)
        cmake_vars_file_opener_mock.open = MagicMock(return_value=cmake_variables_map)

        config = Config()
        config.cmake = Config.CMake()
        config.cmake.directory_name = 'cmake_dir_name'

        target_config = Config.CMake.Target()
        target_config.target_name = 'target_name'
        target_config.headers = Config.CMake.Target.Files
        target_config.headers.base_dir =\
            f'{cmake_variable1_name}/{cmake_variable2_name}/{non_existent_cmake_variable_name}{base_dir_name}'

        sut = TargetConfigManager(cmake_vars_file_opener_mock)
        sut.get_headers_base_directory(target_config, config.cmake)


def test_get_sources_base_directory_will_return_empty_string_when_no_base_directory_defined():
    expected_base_directory_path = ''

    cmake_vars_file_opener_mock = MagicMock(TargetCMakeVarsFileOpener)
    cmake_vars_file_opener_mock.open = MagicMock(return_value={})

    config = Config()
    config.cmake = Config.CMake()

    target_config = Config.CMake.Target()
    target_config.sources = Config.CMake.Target.Files
    target_config.sources.base_dir = ''

    sut = TargetConfigManager(cmake_vars_file_opener_mock)
    actual_base_directory_path = sut.get_sources_base_directory(target_config, config.cmake)

    assert actual_base_directory_path == expected_base_directory_path


def test_get_sources_base_directory_will_return_full_sources_base_directory_path_based_on_base_directory():
    expected_base_directory_path = 'base/dir/path'

    cmake_vars_file_opener_mock = MagicMock(TargetCMakeVarsFileOpener)
    cmake_vars_file_opener_mock.open = MagicMock(return_value={})

    config = Config()
    config.cmake = Config.CMake()
    config.cmake.directory_name = 'cmake_dir_name'

    target_config = Config.CMake.Target()
    target_config.sources = Config.CMake.Target.Files
    target_config.sources.base_dir = expected_base_directory_path

    sut = TargetConfigManager(cmake_vars_file_opener_mock)
    actual_base_directory_path = sut.get_sources_base_directory(target_config, config.cmake)

    assert actual_base_directory_path == expected_base_directory_path


def test_get_sources_base_directory_will_return_full_path_when_base_dir_contains_cmake_variable():
    base_dir_name = 'base_dir'

    cmake_variable_name = '${BEAST_SRC_DIR}'
    cmake_variable_value = 'variable_value'
    cmake_variables_map = {cmake_variable_name: cmake_variable_value}

    cmake_vars_file_opener_mock = MagicMock(TargetCMakeVarsFileOpener)
    cmake_vars_file_opener_mock.open = MagicMock(return_value=cmake_variables_map)

    config = Config()
    config.cmake = Config.CMake()
    config.cmake.directory_name = 'cmake_dir_name'

    target_config = Config.CMake.Target()
    target_config.sources = Config.CMake.Target.Files
    target_config.sources.base_dir = f'{cmake_variable_name}/{base_dir_name}'

    expected_base_directory_path = f'{cmake_variable_value}/{base_dir_name}'

    sut = TargetConfigManager(cmake_vars_file_opener_mock)
    actual_base_directory_path = sut.get_sources_base_directory(target_config, config.cmake)

    assert actual_base_directory_path == expected_base_directory_path


def test_get_sources_base_directory_will_return_full_path_when_base_dir_contains_multiple_cmake_variables():
    base_dir_name = 'base_dir'

    cmake_variable1_name = '${BEAST_SRC_DIR}'
    cmake_variable1_value = 'include_dir'
    cmake_variable2_name = '${BEAST_SOME_VARIABLE}'
    cmake_variable2_value = 'some_variable'

    cmake_variables_map = {cmake_variable1_name: cmake_variable1_value, cmake_variable2_name: cmake_variable2_value}

    cmake_vars_file_opener_mock = MagicMock(TargetCMakeVarsFileOpener)
    cmake_vars_file_opener_mock.open = MagicMock(return_value=cmake_variables_map)

    config = Config()
    config.cmake = Config.CMake()
    config.cmake.directory_name = 'cmake_dir_name'

    target_config = Config.CMake.Target()
    target_config.sources = Config.CMake.Target.Files
    target_config.sources.base_dir = f'{cmake_variable1_name}/{cmake_variable2_name}/{base_dir_name}'

    expected_base_directory_path = f'{cmake_variable1_value}/{cmake_variable2_value}/{base_dir_name}'

    sut = TargetConfigManager(cmake_vars_file_opener_mock)
    actual_base_directory_path = sut.get_sources_base_directory(target_config, config.cmake)

    assert actual_base_directory_path == expected_base_directory_path


def test_get_sources_base_directory_will_throw_exception_when_cmake_variable_from_base_dir_does_not_exist_in_vars_file():
    with pytest.raises(ValueError):
        base_dir_name = 'base_dir'

        cmake_variable1_name = '${BEAST_INCLUDE_DIR}'
        cmake_variable1_value = 'include_dir'
        cmake_variable2_name = '${BEAST_SOME_VARIABLE}'
        cmake_variable2_value = 'some_variable'
        non_existent_cmake_variable_name = '${THIS_VARIABLE_DOES_NOT_EXIST}'

        cmake_variables_map = {cmake_variable1_name: cmake_variable1_value, cmake_variable2_name: cmake_variable2_value}

        cmake_vars_file_opener_mock = MagicMock(TargetCMakeVarsFileOpener)
        cmake_vars_file_opener_mock.open = MagicMock(return_value=cmake_variables_map)

        config = Config()
        config.cmake = Config.CMake()
        config.cmake.directory_name = 'cmake_dir_name'

        target_config = Config.CMake.Target()
        target_config.target_name = 'target_name'
        target_config.sources = Config.CMake.Target.Files
        target_config.sources.base_dir =\
            f'{cmake_variable1_name}/{cmake_variable2_name}/{non_existent_cmake_variable_name}{base_dir_name}'

        sut = TargetConfigManager(cmake_vars_file_opener_mock)
        sut.get_sources_base_directory(target_config, config.cmake)
