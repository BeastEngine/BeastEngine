from unittest.mock import MagicMock

from src.config.ConfigManager import Config, ConfigFiles
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


class MainCMakeConfigData:
    expected_project_name_placeholder = "project_name"
    expected_project_version_major_placeholder = "project_version_major"
    expected_project_version_minor_placeholder = "project_version_minor"
    expected_project_version_patch_placeholder = "project_version_patch"

    expected_lib_target_name_placeholder = "lib_target_name"
    expected_exe_target_name_placeholder = "exe_target_name"

    expected_cmake_config_file_content_before_change = f'''
set(BEAST_PROJECT_NAME {{{expected_project_name_placeholder}}})
set(BEAST_VERSION_MAJOR {{{expected_project_version_major_placeholder}}})
set(BEAST_VERSION_MINOR {{{expected_project_version_minor_placeholder}}})
set(BEAST_VERSION_PATCH {{{expected_project_version_patch_placeholder}}})

set(BEAST_LIB_TARGET_NAME {{{expected_lib_target_name_placeholder}}})
set(BEAST_EXE_TARGET_NAME {{{expected_exe_target_name_placeholder}}})
'''

    expected_project_name = "test_project"
    expected_project_version_major = "1"
    expected_project_version_minor = "0"
    expected_project_version_patch = "2"

    expected_lib_target_name = 'lib'
    expected_exe_target_name = 'exe'

    expected_cmake_file_content_after_change = f'''
set(BEAST_PROJECT_NAME {expected_project_name})
set(BEAST_VERSION_MAJOR {expected_project_version_major})
set(BEAST_VERSION_MINOR {expected_project_version_minor})
set(BEAST_VERSION_PATCH {expected_project_version_patch})

set(BEAST_LIB_TARGET_NAME {expected_lib_target_name})
set(BEAST_EXE_TARGET_NAME {expected_exe_target_name})
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

    cmake_config.project.project_name_placeholder = expected_project_name_placeholder
    cmake_config.project.version_major_placeholder = expected_project_version_major_placeholder
    cmake_config.project.version_minor_placeholder = expected_project_version_minor_placeholder
    cmake_config.project.version_patch_placeholder = expected_project_version_patch_placeholder

    cmake_config.lib = Config.CMake.Target()
    cmake_config.lib.target_name = expected_lib_target_name
    cmake_config.lib.target_name_placeholder = expected_lib_target_name_placeholder

    cmake_config.exe = Config.CMake.Target()
    cmake_config.exe.target_name = expected_exe_target_name
    cmake_config.exe.target_name_placeholder = expected_exe_target_name_placeholder


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
    sut.generate_main_config(config_data.cmake_config, test_data.CMAKE_CONFIG_DIR)

    test_data.command_runner_mock.run_command.assert_called_with(expected_copy_command)
    test_data.file_opener_mock.open.assert_called_with(expected_full_file_path)
    file_mock.replace_content.assert_called_with(config_data.expected_cmake_file_content_after_change)


def test_generate_target_config_will_return_immediately_if_target_config_does_not_contain_config_files_definition():
    test_data = CommonTestData()
    target_config = Config.CMake.Target()
    target_config.config_files = None

    sut = CMakeConfigFilesCreator(test_data.command_runner_mock, test_data.file_opener_mock)
    sut.generate_target_config(target_config, test_data.CMAKE_CONFIG_DIR)

    test_data.command_runner_mock.run_command.assert_not_called()
    test_data.file_opener_mock.open.assert_not_called()


def test_generate_target_config_will_return_immediately_if_target_config_does_not_contain_directories_definition():
    test_data = CommonTestData()
    target_config = Config.CMake.Target()
    target_config.config_files = ConfigFiles()
    target_config.config_files.filename = "filename"
    target_config.config_files.dist_filename = "dist_filename"
    target_config.directories = None

    sut = CMakeConfigFilesCreator(test_data.command_runner_mock, test_data.file_opener_mock)
    sut.generate_target_config(target_config, test_data.CMAKE_CONFIG_DIR)

    test_data.command_runner_mock.run_command.assert_not_called()
    test_data.file_opener_mock.open.assert_not_called()


class TargetCMakeConfigData:
    def __init__(self):
        self.expected_include_dir_placeholder = "target_include_dir"
        self.expected_source_dir_placeholder = "target_src_dir"
        
        self.expected_headers_placeholder = "target_headers"
        self.expected_sources_placeholder = "target_sources"
        
        self.expected_cmake_config_file_content_before_change = f'''
SET(BEAST_INCLUDE_DIR {{{self.expected_include_dir_placeholder}}})
SET(BEAST_SRC_DIR {{{self.expected_source_dir_placeholder}}})

SET(
    BEAST_HEADERS_LIST
    {{{self.expected_headers_placeholder}}}
)
SET(
    BEAST_SRC_LIST
    {{{self.expected_sources_placeholder}}}
)
'''
        
        self.expected_include_dir = "include/dir"
        self.expected_source_dir = "src/dir"
        
        self.expected_header_files = ["file.h"]
        self.expected_source_files = ["file.cpp"]
        
        self.expected_cmake_file_content_after_change = f'''
SET(BEAST_INCLUDE_DIR {self.expected_include_dir})
SET(BEAST_SRC_DIR {self.expected_source_dir})

SET(
    BEAST_HEADERS_LIST
    "file.h"
)
SET(
    BEAST_SRC_LIST
    "file.cpp"
)
'''
        
        self.target_config = Config.CMake.Target()
        self.target_config.config_files = ConfigFiles()
        self.target_config.config_files.filename = "target/config.cmake"
        self.target_config.config_files.dist_filename = "target/config.cmake.dist"
        
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
    sut.generate_target_config(config_data.target_config, test_data.CMAKE_CONFIG_DIR)

    test_data.command_runner_mock.run_command.assert_called_with(expected_copy_command)
    test_data.file_opener_mock.open.assert_called_with(expected_full_file_path)
    file_mock.replace_content.assert_called_with(config_data.expected_cmake_file_content_after_change)


def test_generate_target_config_will_separate_every_header_file_with_new_line_and_indentation():
    test_data = CommonTestData()
    config_data = TargetCMakeConfigData()

    config_data.expected_header_files = ["file1.h", "file2.h", "file3.h"]
    config_data.target_config.headers.files = config_data.expected_header_files

    config_data.expected_cmake_file_content_after_change = f'''
SET(BEAST_INCLUDE_DIR {config_data.expected_include_dir})
SET(BEAST_SRC_DIR {config_data.expected_source_dir})

SET(
    BEAST_HEADERS_LIST
    "file1.h"
    "file2.h"
    "file3.h"
)
SET(
    BEAST_SRC_LIST
    "file.cpp"
)
'''

    file_mock = MagicMock(FileOpener.File)
    file_mock.get_content = MagicMock(return_value=config_data.expected_cmake_config_file_content_before_change)
    file_mock.replace_content = MagicMock()
    test_data.file_opener_mock.open.return_value = file_mock

    expected_full_dist_file_path = f"{test_data.CMAKE_CONFIG_DIR}/{config_data.target_config.config_files.dist_filename}"
    expected_full_file_path = f"{test_data.CMAKE_CONFIG_DIR}/{config_data.target_config.config_files.filename}"
    expected_copy_command = f"cp {expected_full_dist_file_path} {expected_full_file_path}"

    sut = CMakeConfigFilesCreator(test_data.command_runner_mock, test_data.file_opener_mock)
    sut.generate_target_config(config_data.target_config, test_data.CMAKE_CONFIG_DIR)

    test_data.command_runner_mock.run_command.assert_called_with(expected_copy_command)
    test_data.file_opener_mock.open.assert_called_with(expected_full_file_path)
    file_mock.replace_content.assert_called_with(config_data.expected_cmake_file_content_after_change)


def test_generate_target_config_will_separate_every_source_file_with_new_line_and_indentation():
    test_data = CommonTestData()
    config_data = TargetCMakeConfigData()

    config_data.expected_source_files = ["file1.cpp", "file2.cpp", "file3.cpp"]
    config_data.target_config.sources.files = config_data.expected_source_files

    config_data.expected_cmake_file_content_after_change = f'''
SET(BEAST_INCLUDE_DIR {config_data.expected_include_dir})
SET(BEAST_SRC_DIR {config_data.expected_source_dir})

SET(
    BEAST_HEADERS_LIST
    "file.h"
)
SET(
    BEAST_SRC_LIST
    "file1.cpp"
    "file2.cpp"
    "file3.cpp"
)
'''

    file_mock = MagicMock(FileOpener.File)
    file_mock.get_content = MagicMock(return_value=config_data.expected_cmake_config_file_content_before_change)
    file_mock.replace_content = MagicMock()
    test_data.file_opener_mock.open.return_value = file_mock

    expected_full_dist_file_path = f"{test_data.CMAKE_CONFIG_DIR}/{config_data.target_config.config_files.dist_filename}"
    expected_full_file_path = f"{test_data.CMAKE_CONFIG_DIR}/{config_data.target_config.config_files.filename}"
    expected_copy_command = f"cp {expected_full_dist_file_path} {expected_full_file_path}"

    sut = CMakeConfigFilesCreator(test_data.command_runner_mock, test_data.file_opener_mock)
    sut.generate_target_config(config_data.target_config, test_data.CMAKE_CONFIG_DIR)

    test_data.command_runner_mock.run_command.assert_called_with(expected_copy_command)
    test_data.file_opener_mock.open.assert_called_with(expected_full_file_path)
    file_mock.replace_content.assert_called_with(config_data.expected_cmake_file_content_after_change)


def test_generate_target_config_will_add_base_directory_to_every_header_file_if_base_directory_for_headers_defined():
    test_data = CommonTestData()
    config_data = TargetCMakeConfigData()

    config_data.expected_header_files = ["file1.h", "file2.h", "file3.h"]
    config_data.target_config.headers.files = config_data.expected_header_files
    config_data.target_config.headers.base_dir = "basedir"

    config_data.expected_cmake_file_content_after_change = f'''
SET(BEAST_INCLUDE_DIR {config_data.expected_include_dir})
SET(BEAST_SRC_DIR {config_data.expected_source_dir})

SET(
    BEAST_HEADERS_LIST
    "basedir/file1.h"
    "basedir/file2.h"
    "basedir/file3.h"
)
SET(
    BEAST_SRC_LIST
    "file.cpp"
)
'''

    file_mock = MagicMock(FileOpener.File)
    file_mock.get_content = MagicMock(return_value=config_data.expected_cmake_config_file_content_before_change)
    file_mock.replace_content = MagicMock()
    test_data.file_opener_mock.open.return_value = file_mock

    expected_full_dist_file_path = f"{test_data.CMAKE_CONFIG_DIR}/{config_data.target_config.config_files.dist_filename}"
    expected_full_file_path = f"{test_data.CMAKE_CONFIG_DIR}/{config_data.target_config.config_files.filename}"
    expected_copy_command = f"cp {expected_full_dist_file_path} {expected_full_file_path}"

    sut = CMakeConfigFilesCreator(test_data.command_runner_mock, test_data.file_opener_mock)
    sut.generate_target_config(config_data.target_config, test_data.CMAKE_CONFIG_DIR)

    test_data.command_runner_mock.run_command.assert_called_with(expected_copy_command)
    test_data.file_opener_mock.open.assert_called_with(expected_full_file_path)
    file_mock.replace_content.assert_called_with(config_data.expected_cmake_file_content_after_change)


def test_generate_target_config_will_add_base_directory_to_every_source_file_if_base_directory_for_headers_defined():
    test_data = CommonTestData()
    config_data = TargetCMakeConfigData()

    config_data.expected_source_files = ["file1.cpp", "file2.cpp", "some_dir/file3.cpp"]
    config_data.target_config.sources.files = config_data.expected_source_files
    config_data.target_config.sources.base_dir = "basedir"

    config_data.expected_cmake_file_content_after_change = f'''
SET(BEAST_INCLUDE_DIR {config_data.expected_include_dir})
SET(BEAST_SRC_DIR {config_data.expected_source_dir})

SET(
    BEAST_HEADERS_LIST
    "file.h"
)
SET(
    BEAST_SRC_LIST
    "basedir/file1.cpp"
    "basedir/file2.cpp"
    "basedir/some_dir/file3.cpp"
)
'''

    file_mock = MagicMock(FileOpener.File)
    file_mock.get_content = MagicMock(return_value=config_data.expected_cmake_config_file_content_before_change)
    file_mock.replace_content = MagicMock()
    test_data.file_opener_mock.open.return_value = file_mock

    expected_full_dist_file_path = f"{test_data.CMAKE_CONFIG_DIR}/{config_data.target_config.config_files.dist_filename}"
    expected_full_file_path = f"{test_data.CMAKE_CONFIG_DIR}/{config_data.target_config.config_files.filename}"
    expected_copy_command = f"cp {expected_full_dist_file_path} {expected_full_file_path}"

    sut = CMakeConfigFilesCreator(test_data.command_runner_mock, test_data.file_opener_mock)
    sut.generate_target_config(config_data.target_config, test_data.CMAKE_CONFIG_DIR)

    test_data.command_runner_mock.run_command.assert_called_with(expected_copy_command)
    test_data.file_opener_mock.open.assert_called_with(expected_full_file_path)
    file_mock.replace_content.assert_called_with(config_data.expected_cmake_file_content_after_change)