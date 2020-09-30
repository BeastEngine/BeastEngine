from unittest.mock import MagicMock

import pytest

from src.config.config import Config, ConfigFiles
from src.files.file_opener import FileOpener
from src.commandrunners.command_runner import CommandRunner
from src.commandrunners.cmake.cmake_config_files_creator import CMakeConfigFilesCreator


class CommonTestData:
    CMAKE_CONFIG_DIR = "project/dir/cmake/config"

    def __init__(self):
        self.command_runner_mock = MagicMock(CommandRunner)
        self.command_runner_mock.run_command = MagicMock()

        self.file_opener_mock = MagicMock(FileOpener)
        self.file_opener_mock.open = MagicMock()
        self.verbose = False


class MainCMakeConfigData:
    expected_project_name_placeholder = "project_name"
    expected_project_version_major_placeholder = "project_version_major"
    expected_project_version_minor_placeholder = "project_version_minor"
    expected_project_version_patch_placeholder = "project_version_patch"

    expected_lib_target_name_placeholder = "lib_target_name"
    expected_exe_target_name_placeholder = "exe_target_name"
    expected_tests_target_name_placeholder = "tests_target_name"

    expected_cmake_config_file_content_before_change = f'''
set(BEAST_PROJECT_NAME {{{expected_project_name_placeholder}}})
set(BEAST_VERSION_MAJOR {{{expected_project_version_major_placeholder}}})
set(BEAST_VERSION_MINOR {{{expected_project_version_minor_placeholder}}})
set(BEAST_VERSION_PATCH {{{expected_project_version_patch_placeholder}}})

set(BEAST_LIB_TARGET_NAME {{{expected_lib_target_name_placeholder}}})
set(BEAST_EXE_TARGET_NAME {{{expected_exe_target_name_placeholder}}})
set(BEAST_TESTS_TARGET_NAME {{{expected_tests_target_name_placeholder}}})
'''

    expected_project_name = "test_project"
    expected_project_version_major = "1"
    expected_project_version_minor = "0"
    expected_project_version_patch = "2"

    expected_lib_target_name = 'lib'
    expected_exe_target_name = 'exe'
    expected_tests_target_name = 'tests'

    expected_cmake_file_content_after_change = f'''
set(BEAST_PROJECT_NAME {expected_project_name})
set(BEAST_VERSION_MAJOR {expected_project_version_major})
set(BEAST_VERSION_MINOR {expected_project_version_minor})
set(BEAST_VERSION_PATCH {expected_project_version_patch})

set(BEAST_LIB_TARGET_NAME {expected_lib_target_name})
set(BEAST_EXE_TARGET_NAME {expected_exe_target_name})
set(BEAST_TESTS_TARGET_NAME {expected_tests_target_name})
'''

    cmake_config = Config.CMake()
    cmake_config.config_files = ConfigFiles()
    cmake_config.config_files.dist_filename = "dist_filename"
    cmake_config.config_files.filename = "filename"

    cmake_config.project = Config.CMake.Project()
    cmake_config.project.name = expected_project_name
    cmake_config.project.version_major = expected_project_version_major
    cmake_config.project.version_minor = expected_project_version_minor
    cmake_config.project.version_patch = expected_project_version_patch

    cmake_config.project.name_placeholder = expected_project_name_placeholder
    cmake_config.project.version_major_placeholder = expected_project_version_major_placeholder
    cmake_config.project.version_minor_placeholder = expected_project_version_minor_placeholder
    cmake_config.project.version_patch_placeholder = expected_project_version_patch_placeholder

    cmake_config.lib = Config.CMake.Target()
    cmake_config.lib.target_name = expected_lib_target_name
    cmake_config.lib.target_name_placeholder = expected_lib_target_name_placeholder

    cmake_config.exe = Config.CMake.Target()
    cmake_config.exe.target_name = expected_exe_target_name
    cmake_config.exe.target_name_placeholder = expected_exe_target_name_placeholder

    cmake_config.tests = Config.CMake.Target()
    cmake_config.tests.target_name = expected_tests_target_name
    cmake_config.tests.target_name_placeholder = expected_tests_target_name_placeholder


def test_generate_main_config_will_copy_dist_config_and_replace_its_content_with_proper_values_based_on_given_config_object():
    test_data = CommonTestData()
    config_data = MainCMakeConfigData()

    file_mock = MagicMock(FileOpener.File)
    file_mock.get_content = MagicMock(return_value=config_data.expected_cmake_config_file_content_before_change)
    file_mock.replace_content = MagicMock()
    test_data.file_opener_mock.open.return_value = file_mock

    expected_full_dist_file_path = f"{test_data.CMAKE_CONFIG_DIR}/dist_filename"
    expected_full_file_path = f"{test_data.CMAKE_CONFIG_DIR}/filename"
    expected_copy_command = f"cp {expected_full_dist_file_path} {expected_full_file_path}"

    sut = CMakeConfigFilesCreator(test_data.command_runner_mock, test_data.file_opener_mock)
    sut.generate_main_config(config_data.cmake_config, test_data.CMAKE_CONFIG_DIR, test_data.verbose)

    test_data.command_runner_mock.run_command.assert_called_with(expected_copy_command, test_data.CMAKE_CONFIG_DIR, test_data.verbose)
    test_data.file_opener_mock.open.assert_called_with(expected_full_file_path)
    file_mock.replace_content.assert_called_with(config_data.expected_cmake_file_content_after_change)


def test_generate_target_config_will_return_immediately_if_target_config_does_not_contain_config_files_definition():
    test_data = CommonTestData()
    target_config = TargetCMakeConfigData().target_config
    target_config.config_files = None

    sut = CMakeConfigFilesCreator(test_data.command_runner_mock, test_data.file_opener_mock)
    sut.generate_target_config(target_config, test_data.CMAKE_CONFIG_DIR, test_data.verbose)

    test_data.command_runner_mock.run_command.assert_not_called()
    test_data.file_opener_mock.open.assert_not_called()


def test_generate_target_config_will_return_immediately_if_target_config_does_not_contain_variables_definition():
    test_data = CommonTestData()
    target_config = TargetCMakeConfigData().target_config
    target_config.variables = None

    sut = CMakeConfigFilesCreator(test_data.command_runner_mock, test_data.file_opener_mock)
    sut.generate_target_config(target_config, test_data.CMAKE_CONFIG_DIR, test_data.verbose)

    test_data.command_runner_mock.run_command.assert_not_called()
    test_data.file_opener_mock.open.assert_not_called()


def test_generate_target_config_will_return_immediately_if_target_config_does_not_contain_directories_definition():
    test_data = CommonTestData()
    target_config = TargetCMakeConfigData().target_config
    target_config.directories = None

    sut = CMakeConfigFilesCreator(test_data.command_runner_mock, test_data.file_opener_mock)
    sut.generate_target_config(target_config, test_data.CMAKE_CONFIG_DIR, test_data.verbose)

    test_data.command_runner_mock.run_command.assert_not_called()
    test_data.file_opener_mock.open.assert_not_called()


@pytest.mark.parametrize('expected_verbose', [True, False])
def test_generate_main_config_will_copy_dist_config_with_passed_verbose_option(expected_verbose):
    test_data = CommonTestData()
    config_data = MainCMakeConfigData()
    verbose_argument_in_method_call_index = 2

    sut = CMakeConfigFilesCreator(test_data.command_runner_mock, test_data.file_opener_mock)
    sut.generate_main_config(config_data.cmake_config, test_data.CMAKE_CONFIG_DIR, expected_verbose)

    command_runner_call = test_data.command_runner_mock.run_command.call_args
    call_args = command_runner_call[0]
    assert call_args[verbose_argument_in_method_call_index] == expected_verbose


class TargetCMakeConfigData:
    def __init__(self):
        self.expected_include_dir_placeholder = "target_include_dir"
        self.expected_source_dir_placeholder = "target_src_dir"
        
        self.expected_headers_placeholder = "target_headers"
        self.expected_sources_placeholder = "target_sources"

        self.expected_target_cmake_variables_file_path_placeholder = "target_cmake_variables_file_path_placeholder"
        self.expected_target_cmake_variables_file_path = "target/cmake/variables/file/path"

        self.expected_cmake_config_file_content_before_change = f'''
set(BEAST_INCLUDE_DIR {{{self.expected_include_dir_placeholder}}})
set(BEAST_SRC_DIR {{{self.expected_source_dir_placeholder}}})

set(
    BEAST_HEADERS_LIST
    {{{self.expected_headers_placeholder}}}
)
set(
    BEAST_SRC_LIST
    {{{self.expected_sources_placeholder}}}
)

file(WRITE {{{self.expected_target_cmake_variables_file_path_placeholder}}} "some_variable")
'''

        self.expected_include_dir = "include/dir"
        self.expected_source_dir = "src/dir"

        self.expected_header_files = ["file.h"]
        self.expected_source_files = ["file.cpp"]

        self.expected_cmake_file_content_after_change = f'''
set(BEAST_INCLUDE_DIR {self.expected_include_dir})
set(BEAST_SRC_DIR {self.expected_source_dir})

set(
    BEAST_HEADERS_LIST
    "file.h"
)
set(
    BEAST_SRC_LIST
    "file.cpp"
)

file(WRITE "{CommonTestData.CMAKE_CONFIG_DIR}/{self.expected_target_cmake_variables_file_path}" "some_variable")
'''

        self.target_config = Config.CMake.Target()
        self.target_config.config_files = ConfigFiles()
        self.target_config.config_files.filename = "target/config.cmake"
        self.target_config.config_files.dist_filename = "target/config.cmake.dist"

        self.target_config.variables = Config.CMake.Target.Variables()
        self.target_config.variables.target_cmake_variables_file_path_placeholder = self.expected_target_cmake_variables_file_path_placeholder
        self.target_config.variables.target_cmake_variables_file_path = self.expected_target_cmake_variables_file_path

        self.target_config.directories = Config.CMake.Target.Directories()
        self.target_config.directories.include_directory_placeholder = self.expected_include_dir_placeholder
        self.target_config.directories.include_directory = self.expected_include_dir
        self.target_config.directories.source_directory_placeholder = self.expected_source_dir_placeholder
        self.target_config.directories.source_directory = self.expected_source_dir

        self.target_config.headers = Config.CMake.Target.Files()
        self.target_config.headers.base_dir = ""
        self.target_config.headers.files_list_placeholder = self.expected_headers_placeholder
        self.target_config.headers.files = self.expected_header_files

        self.target_config.sources = Config.CMake.Target.Files()
        self.target_config.sources.base_dir = ""
        self.target_config.sources.files_list_placeholder = self.expected_sources_placeholder
        self.target_config.sources.files = self.expected_source_files


def test_generate_target_config_will_copy_dist_config_and_replace_its_content_with_proper_values_based_on_given_config_object():
    test_data = CommonTestData()
    config_data = TargetCMakeConfigData()

    file_mock = MagicMock(FileOpener.File)
    file_mock.get_content = MagicMock(return_value=config_data.expected_cmake_config_file_content_before_change)
    file_mock.replace_content = MagicMock()
    test_data.file_opener_mock.open.return_value = file_mock

    expected_full_dist_file_path = f"{test_data.CMAKE_CONFIG_DIR}/{config_data.target_config.config_files.dist_filename}"
    expected_full_file_path = f"{test_data.CMAKE_CONFIG_DIR}/{config_data.target_config.config_files.filename}"
    expected_copy_command = f"cp {expected_full_dist_file_path} {expected_full_file_path}"

    sut = CMakeConfigFilesCreator(test_data.command_runner_mock, test_data.file_opener_mock)
    sut.generate_target_config(config_data.target_config, test_data.CMAKE_CONFIG_DIR, test_data.verbose)

    test_data.command_runner_mock.run_command.assert_called_with(expected_copy_command, test_data.CMAKE_CONFIG_DIR, test_data.verbose)
    test_data.file_opener_mock.open.assert_called_with(expected_full_file_path)
    file_mock.replace_content.assert_called_with(config_data.expected_cmake_file_content_after_change)


def test_generate_target_config_will_separate_every_header_file_with_new_line_and_indentation():
    test_data = CommonTestData()
    config_data = TargetCMakeConfigData()

    config_data.expected_header_files = ["file1.h", "file2.h", "file3.h"]
    config_data.target_config.headers.files = config_data.expected_header_files

    config_data.expected_cmake_file_content_after_change = f'''
set(BEAST_INCLUDE_DIR {config_data.expected_include_dir})
set(BEAST_SRC_DIR {config_data.expected_source_dir})

set(
    BEAST_HEADERS_LIST
    "file1.h"
    "file2.h"
    "file3.h"
)
set(
    BEAST_SRC_LIST
    "file.cpp"
)

file(WRITE "{test_data.CMAKE_CONFIG_DIR}/{config_data.expected_target_cmake_variables_file_path}" "some_variable")
'''

    file_mock = MagicMock(FileOpener.File)
    file_mock.get_content = MagicMock(return_value=config_data.expected_cmake_config_file_content_before_change)
    file_mock.replace_content = MagicMock()
    test_data.file_opener_mock.open.return_value = file_mock

    expected_full_dist_file_path = f"{test_data.CMAKE_CONFIG_DIR}/{config_data.target_config.config_files.dist_filename}"
    expected_full_file_path = f"{test_data.CMAKE_CONFIG_DIR}/{config_data.target_config.config_files.filename}"
    expected_copy_command = f"cp {expected_full_dist_file_path} {expected_full_file_path}"

    sut = CMakeConfigFilesCreator(test_data.command_runner_mock, test_data.file_opener_mock)
    sut.generate_target_config(config_data.target_config, test_data.CMAKE_CONFIG_DIR, test_data.verbose)

    test_data.command_runner_mock.run_command.assert_called_with(expected_copy_command, test_data.CMAKE_CONFIG_DIR, test_data.verbose)
    test_data.file_opener_mock.open.assert_called_with(expected_full_file_path)
    file_mock.replace_content.assert_called_with(config_data.expected_cmake_file_content_after_change)


def test_generate_target_config_will_separate_every_source_file_with_new_line_and_indentation():
    test_data = CommonTestData()
    config_data = TargetCMakeConfigData()

    config_data.expected_source_files = ["file1.cpp", "file2.cpp", "file3.cpp"]
    config_data.target_config.sources.files = config_data.expected_source_files

    config_data.expected_cmake_file_content_after_change = f'''
set(BEAST_INCLUDE_DIR {config_data.expected_include_dir})
set(BEAST_SRC_DIR {config_data.expected_source_dir})

set(
    BEAST_HEADERS_LIST
    "file.h"
)
set(
    BEAST_SRC_LIST
    "file1.cpp"
    "file2.cpp"
    "file3.cpp"
)

file(WRITE "{test_data.CMAKE_CONFIG_DIR}/{config_data.expected_target_cmake_variables_file_path}" "some_variable")
'''

    file_mock = MagicMock(FileOpener.File)
    file_mock.get_content = MagicMock(return_value=config_data.expected_cmake_config_file_content_before_change)
    file_mock.replace_content = MagicMock()
    test_data.file_opener_mock.open.return_value = file_mock

    expected_full_dist_file_path = f"{test_data.CMAKE_CONFIG_DIR}/{config_data.target_config.config_files.dist_filename}"
    expected_full_file_path = f"{test_data.CMAKE_CONFIG_DIR}/{config_data.target_config.config_files.filename}"
    expected_copy_command = f"cp {expected_full_dist_file_path} {expected_full_file_path}"

    sut = CMakeConfigFilesCreator(test_data.command_runner_mock, test_data.file_opener_mock)
    sut.generate_target_config(config_data.target_config, test_data.CMAKE_CONFIG_DIR, test_data.verbose)

    test_data.command_runner_mock.run_command.assert_called_with(expected_copy_command, test_data.CMAKE_CONFIG_DIR, test_data.verbose)
    test_data.file_opener_mock.open.assert_called_with(expected_full_file_path)
    file_mock.replace_content.assert_called_with(config_data.expected_cmake_file_content_after_change)


def test_generate_target_config_will_add_base_directory_to_every_header_file_if_base_directory_for_headers_defined():
    test_data = CommonTestData()
    config_data = TargetCMakeConfigData()

    config_data.expected_header_files = ["file1.h", "file2.h", "file3.h"]
    config_data.target_config.headers.files = config_data.expected_header_files
    config_data.target_config.headers.base_dir = "basedir"

    config_data.expected_cmake_file_content_after_change = f'''
set(BEAST_INCLUDE_DIR {config_data.expected_include_dir})
set(BEAST_SRC_DIR {config_data.expected_source_dir})

set(
    BEAST_HEADERS_LIST
    "basedir/file1.h"
    "basedir/file2.h"
    "basedir/file3.h"
)
set(
    BEAST_SRC_LIST
    "file.cpp"
)

file(WRITE "{test_data.CMAKE_CONFIG_DIR}/{config_data.expected_target_cmake_variables_file_path}" "some_variable")
'''

    file_mock = MagicMock(FileOpener.File)
    file_mock.get_content = MagicMock(return_value=config_data.expected_cmake_config_file_content_before_change)
    file_mock.replace_content = MagicMock()
    test_data.file_opener_mock.open.return_value = file_mock

    expected_full_dist_file_path = f"{test_data.CMAKE_CONFIG_DIR}/{config_data.target_config.config_files.dist_filename}"
    expected_full_file_path = f"{test_data.CMAKE_CONFIG_DIR}/{config_data.target_config.config_files.filename}"
    expected_copy_command = f"cp {expected_full_dist_file_path} {expected_full_file_path}"

    sut = CMakeConfigFilesCreator(test_data.command_runner_mock, test_data.file_opener_mock)
    sut.generate_target_config(config_data.target_config, test_data.CMAKE_CONFIG_DIR, test_data.verbose)

    test_data.command_runner_mock.run_command.assert_called_with(expected_copy_command, test_data.CMAKE_CONFIG_DIR, test_data.verbose)
    test_data.file_opener_mock.open.assert_called_with(expected_full_file_path)
    file_mock.replace_content.assert_called_with(config_data.expected_cmake_file_content_after_change)


def test_generate_target_config_will_add_base_directory_to_every_source_file_if_base_directory_for_headers_defined():
    test_data = CommonTestData()
    config_data = TargetCMakeConfigData()

    config_data.expected_source_files = ["file1.cpp", "file2.cpp", "some_dir/file3.cpp"]
    config_data.target_config.sources.files = config_data.expected_source_files
    config_data.target_config.sources.base_dir = "basedir"

    config_data.expected_cmake_file_content_after_change = f'''
set(BEAST_INCLUDE_DIR {config_data.expected_include_dir})
set(BEAST_SRC_DIR {config_data.expected_source_dir})

set(
    BEAST_HEADERS_LIST
    "file.h"
)
set(
    BEAST_SRC_LIST
    "basedir/file1.cpp"
    "basedir/file2.cpp"
    "basedir/some_dir/file3.cpp"
)

file(WRITE "{test_data.CMAKE_CONFIG_DIR}/{config_data.expected_target_cmake_variables_file_path}" "some_variable")
'''

    file_mock = MagicMock(FileOpener.File)
    file_mock.get_content = MagicMock(return_value=config_data.expected_cmake_config_file_content_before_change)
    file_mock.replace_content = MagicMock()
    test_data.file_opener_mock.open.return_value = file_mock

    expected_full_dist_file_path = f"{test_data.CMAKE_CONFIG_DIR}/{config_data.target_config.config_files.dist_filename}"
    expected_full_file_path = f"{test_data.CMAKE_CONFIG_DIR}/{config_data.target_config.config_files.filename}"
    expected_copy_command = f"cp {expected_full_dist_file_path} {expected_full_file_path}"

    sut = CMakeConfigFilesCreator(test_data.command_runner_mock, test_data.file_opener_mock)
    sut.generate_target_config(config_data.target_config, test_data.CMAKE_CONFIG_DIR, test_data.verbose)

    test_data.command_runner_mock.run_command.assert_called_with(expected_copy_command, test_data.CMAKE_CONFIG_DIR, test_data.verbose)
    test_data.file_opener_mock.open.assert_called_with(expected_full_file_path)
    file_mock.replace_content.assert_called_with(config_data.expected_cmake_file_content_after_change)


def test_generate_target_config_will_replace_target_cmake_variables_file_path_placeholder_with_proper_one():
    test_data = CommonTestData()
    config_data = TargetCMakeConfigData()

    config_data.expected_cmake_file_content_after_change = f'''
set(BEAST_INCLUDE_DIR {config_data.expected_include_dir})
set(BEAST_SRC_DIR {config_data.expected_source_dir})

set(
    BEAST_HEADERS_LIST
    "file.h"
)
set(
    BEAST_SRC_LIST
    "file.cpp"
)

file(WRITE "{test_data.CMAKE_CONFIG_DIR}/{config_data.expected_target_cmake_variables_file_path}" "some_variable")
'''

    file_mock = MagicMock(FileOpener.File)
    file_mock.get_content = MagicMock(return_value=config_data.expected_cmake_config_file_content_before_change)
    file_mock.replace_content = MagicMock()
    test_data.file_opener_mock.open.return_value = file_mock

    sut = CMakeConfigFilesCreator(test_data.command_runner_mock, test_data.file_opener_mock)
    sut.generate_target_config(config_data.target_config, test_data.CMAKE_CONFIG_DIR, test_data.verbose)

    file_mock.replace_content.assert_called_with(config_data.expected_cmake_file_content_after_change)


@pytest.mark.parametrize('expected_verbose', [True, False])
def test_generate_target_config_will_copy_dist_config_with_passed_verbose_option(expected_verbose):
    test_data = CommonTestData()
    config_data = TargetCMakeConfigData()
    verbose_argument_in_method_call_index = 2

    sut = CMakeConfigFilesCreator(test_data.command_runner_mock, test_data.file_opener_mock)
    sut.generate_target_config(config_data.target_config, test_data.CMAKE_CONFIG_DIR, expected_verbose)

    command_runner_call = test_data.command_runner_mock.run_command.call_args
    call_args = command_runner_call[0]
    assert call_args[verbose_argument_in_method_call_index] == expected_verbose
