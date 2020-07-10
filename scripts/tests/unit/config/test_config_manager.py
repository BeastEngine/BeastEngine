import copy
import json
from unittest.mock import MagicMock

from src.config.config_manager import ConfigManager, Config
from src.json_utils.json_manager import JSONManager


class CommonTestData:
    def __init__(self):
        self.json_manager_mock = MagicMock(JSONManager)
        self.json_manager_mock.load_from_file = MagicMock()
        self.config_json = \
            {
                "default_build_type": "Debug",
                "cmake_config": {
                    "directory_name": "cmake/config",
                    "config_files": {
                        "dist_filename": "config.cmake.dist",
                        "filename": "config.cmake"
                    },
                    "project": {
                        "name": "BeastEngine",
                        "version_major": "0",
                        "version_minor": "0",
                        "version_patch": "1",
                        "name_placeholder": "project_name",
                        "version_major_placeholder": "project_version_major",
                        "version_minor_placeholder": "project_version_minor",
                        "version_patch_placeholder": "project_version_patch"
                    },
                    "targets": {
                        "lib": {
                            "target_name": "beastengine",
                            "target_name_placeholder": "lib_target_name",
                            "config_files": {
                                "dist_filename": "beastengine/config.cmake.dist",
                                "filename": "beastengine/config.cmake"
                            },
                            "variables": {
                                "target_cmake_variables_file_path_placeholder": "beast_cmake_vars_file_path",
                                "target_cmake_variables_file_path": "beastengine/beast_vars.txt",
                            },
                            "directories": {
                                "include_directory_placeholder": "beast_include_dir",
                                "include_directory": "\"${BeastEngine_SOURCE_DIR}/include\"",
                                "source_directory_placeholder": "beast_src_dir",
                                "source_directory": "\"${BeastEngine_SOURCE_DIR}/src\""
                            },
                            "headers": {
                                "base_dir": "${BEAST_INCLUDE_DIR}/BeastEngine",
                                "files_list_placeholder": "beast_headers",
                                "files": [
                                    "beastengine.h"
                                ]
                            },
                            "sources": {
                                "base_dir": "${BEAST_SRC_DIR}/BeastEngine",
                                "files_list_placeholder": "beast_sources",
                                "files": [
                                    "beastengine.cpp"
                                ]
                            }
                        },
                        "exe": {
                            "target_name": "sandbox",
                            "target_name_placeholder": "exe_target_name",
                            "config_files": None,
                            "variables": None,
                            "directories": None,
                            "headers": {
                                "base_dir": "",
                                "files_list_placeholder": "",
                                "files": []
                            },
                            "sources": {
                                "base_dir": "",
                                "files_list_placeholder": "",
                                "files": []
                            }
                        }
                    }
                }
            }
        self.config_content = json.dumps(self.config_json)


def test_init_will_load_config_json_from_given_file():
    test_data = CommonTestData()

    expected_file_path = "path/to/json/file/json"
    test_data.json_manager_mock.load_from_file.return_value = test_data.config_json

    ConfigManager(expected_file_path, test_data.json_manager_mock)
    test_data.json_manager_mock.load_from_file.assert_called_with(expected_file_path)


def test_init_will_generate_config_class_with_proper_default_build_type():
    test_data = CommonTestData()
    file_path = "path/to/json/file/json"

    test_data.json_manager_mock.load_from_file.return_value = test_data.config_json

    sut = ConfigManager(file_path, test_data.json_manager_mock)
    actual_config = sut.config

    assert actual_config.default_build_type == test_data.config_json['default_build_type']


def test_init_will_generate_config_class_with_proper_cmake_config():
    test_data = CommonTestData()
    file_path = "path/to/json/file/json"

    test_data.json_manager_mock.load_from_file.return_value = test_data.config_json
    expected_config = test_data.config_json['cmake_config']

    sut = ConfigManager(file_path, test_data.json_manager_mock)
    actual_config = sut.config.cmake

    assert actual_config.directory_name == expected_config['directory_name']
    assert actual_config.config_files.filename == expected_config['config_files']['filename']
    assert actual_config.config_files.dist_filename == expected_config['config_files']['dist_filename']


def test_init_will_generate_config_class_with_proper_cmake_project_config():
    test_data = CommonTestData()
    file_path = "path/to/json/file/json"

    test_data.json_manager_mock.load_from_file.return_value = test_data.config_json
    expected_config = test_data.config_json['cmake_config']['project']

    sut = ConfigManager(file_path, test_data.json_manager_mock)
    actual_config = sut.config.cmake.project

    assert actual_config.name_placeholder == expected_config['name_placeholder']
    assert actual_config.name == expected_config['name']
    assert actual_config.version_major_placeholder == expected_config['version_major_placeholder']
    assert actual_config.version_major == expected_config['version_major']
    assert actual_config.version_minor_placeholder == expected_config['version_minor_placeholder']
    assert actual_config.version_minor == expected_config['version_minor']
    assert actual_config.version_patch_placeholder == expected_config['version_patch_placeholder']
    assert actual_config.version_patch == expected_config['version_patch']


def test_init_will_generate_config_class_with_proper_cmake_lib_target_config():
    test_data = CommonTestData()
    file_path = "path/to/json/file/json"

    test_data.json_manager_mock.load_from_file.return_value = test_data.config_json
    expected_config = test_data.config_json['cmake_config']['targets']['lib']

    sut = ConfigManager(file_path, test_data.json_manager_mock)
    actual_config = sut.config.cmake.lib

    assert actual_config.target_name == expected_config['target_name']
    assert actual_config.target_name_placeholder == expected_config['target_name_placeholder']


def test_init_will_generate_config_class_with_proper_cmake_lib_target_config_files_config():
    test_data = CommonTestData()
    file_path = "path/to/json/file/json"

    test_data.json_manager_mock.load_from_file.return_value = test_data.config_json
    expected_config = test_data.config_json['cmake_config']['targets']['lib']['config_files']

    sut = ConfigManager(file_path, test_data.json_manager_mock)
    actual_config = sut.config.cmake.lib.config_files

    assert actual_config.dist_filename == expected_config['dist_filename']
    assert actual_config.filename == expected_config['filename']


def test_init_will_generate_config_class_with_proper_cmake_lib_target_variables_config():
    test_data = CommonTestData()
    file_path = "path/to/json/file/json"

    test_data.json_manager_mock.load_from_file.return_value = test_data.config_json
    expected_config = test_data.config_json['cmake_config']['targets']['lib']['variables']

    sut = ConfigManager(file_path, test_data.json_manager_mock)
    actual_config = sut.config.cmake.lib.variables

    assert actual_config.target_cmake_variables_file_path_placeholder == expected_config['target_cmake_variables_file_path_placeholder']
    assert actual_config.target_cmake_variables_file_path == expected_config['target_cmake_variables_file_path']


def test_init_will_generate_config_class_with_proper_cmake_lib_target_directories_config():
    test_data = CommonTestData()
    file_path = "path/to/json/file/json"

    test_data.json_manager_mock.load_from_file.return_value = test_data.config_json
    expected_config = test_data.config_json['cmake_config']['targets']['lib']['directories']

    sut = ConfigManager(file_path, test_data.json_manager_mock)
    actual_config = sut.config.cmake.lib.directories

    assert actual_config.source_directory == expected_config['source_directory']
    assert actual_config.source_directory_placeholder == expected_config['source_directory_placeholder']
    assert actual_config.include_directory == expected_config['include_directory']
    assert actual_config.include_directory_placeholder == expected_config['include_directory_placeholder']


def test_init_will_generate_config_class_with_proper_cmake_lib_target_headers_config():
    test_data = CommonTestData()
    file_path = "path/to/json/file/json"

    test_data.json_manager_mock.load_from_file.return_value = test_data.config_json
    expected_config = test_data.config_json['cmake_config']['targets']['lib']['headers']

    sut = ConfigManager(file_path, test_data.json_manager_mock)
    actual_config = sut.config.cmake.lib.headers

    assert actual_config.base_dir == expected_config['base_dir']
    assert actual_config.files == expected_config['files']
    assert actual_config.files_list_placeholder == expected_config['files_list_placeholder']


def test_init_will_generate_config_class_with_proper_cmake_lib_target_sources_config():
    test_data = CommonTestData()
    file_path = "path/to/json/file/json"

    test_data.json_manager_mock.load_from_file.return_value = test_data.config_json
    expected_config = test_data.config_json['cmake_config']['targets']['lib']['sources']

    sut = ConfigManager(file_path, test_data.json_manager_mock)
    actual_config = sut.config.cmake.lib.sources

    assert actual_config.base_dir == expected_config['base_dir']
    assert actual_config.files == expected_config['files']
    assert actual_config.files_list_placeholder == expected_config['files_list_placeholder']


def test_init_will_generate_config_class_with_proper_cmake_exe_target_config():
    test_data = CommonTestData()
    file_path = "path/to/json/file/json"

    test_data.json_manager_mock.load_from_file.return_value = test_data.config_json
    expected_config = test_data.config_json['cmake_config']['targets']['exe']

    sut = ConfigManager(file_path, test_data.json_manager_mock)
    actual_config = sut.config.cmake.exe

    assert actual_config.target_name == expected_config['target_name']
    assert actual_config.target_name_placeholder == expected_config['target_name_placeholder']


def test_init_will_generate_config_class_with_proper_cmake_exe_target_config_files_config():
    test_data = CommonTestData()
    file_path = "path/to/json/file/json"

    test_data.json_manager_mock.load_from_file.return_value = test_data.config_json
    expected_config = test_data.config_json['cmake_config']['targets']['exe']['config_files']

    sut = ConfigManager(file_path, test_data.json_manager_mock)
    actual_config = sut.config.cmake.exe.config_files

    assert actual_config == expected_config


def test_init_will_generate_config_class_with_proper_cmake_exe_target_variables_config():
    test_data = CommonTestData()
    file_path = "path/to/json/file/json"

    test_data.json_manager_mock.load_from_file.return_value = test_data.config_json
    expected_config = test_data.config_json['cmake_config']['targets']['exe']['variables']

    sut = ConfigManager(file_path, test_data.json_manager_mock)
    actual_config = sut.config.cmake.exe.variables

    assert actual_config == expected_config


def test_init_will_generate_config_class_with_proper_cmake_exe_target_directories_config():
    test_data = CommonTestData()
    file_path = "path/to/json/file/json"

    test_data.json_manager_mock.load_from_file.return_value = test_data.config_json
    expected_config = test_data.config_json['cmake_config']['targets']['exe']['directories']

    sut = ConfigManager(file_path, test_data.json_manager_mock)
    actual_config = sut.config.cmake.exe.directories

    assert actual_config == expected_config


def test_init_will_generate_config_class_with_proper_cmake_exe_target_headers_config():
    test_data = CommonTestData()
    file_path = "path/to/json/file/json"

    test_data.json_manager_mock.load_from_file.return_value = test_data.config_json
    expected_config = test_data.config_json['cmake_config']['targets']['exe']['headers']

    sut = ConfigManager(file_path, test_data.json_manager_mock)
    actual_config = sut.config.cmake.exe.headers

    assert actual_config.base_dir == expected_config['base_dir']
    assert actual_config.files == expected_config['files']
    assert actual_config.files_list_placeholder == expected_config['files_list_placeholder']


def test_init_will_generate_config_class_with_proper_cmake_exe_target_sources_config():
    test_data = CommonTestData()
    file_path = "path/to/json/file/json"

    test_data.json_manager_mock.load_from_file.return_value = test_data.config_json
    expected_config = test_data.config_json['cmake_config']['targets']['exe']['sources']

    sut = ConfigManager(file_path, test_data.json_manager_mock)
    actual_config = sut.config.cmake.exe.sources

    assert actual_config.base_dir == expected_config['base_dir']
    assert actual_config.files == expected_config['files']
    assert actual_config.files_list_placeholder == expected_config['files_list_placeholder']


def test_update_config_will_save_updated_config_object_into_selected_config_file():
    test_data = CommonTestData()
    file_path = "path/to/json/file/json"

    expected_config_after = Config()
    expected_config_after.default_build_type = 'BuildType'
    expected_config_after.cmake = None

    test_data.json_manager_mock.load_from_file.return_value = test_data.config_json
    test_data.json_manager_mock.save_to_file = MagicMock()

    sut = ConfigManager(file_path, test_data.json_manager_mock)
    config_before = copy.deepcopy(sut.config)

    sut.config = expected_config_after
    sut.update_config()

    test_data.json_manager_mock.save_to_file.assert_called_with(sut.json_config, file_path, sut.JSON_STR_INDENT)
    assert config_before != sut.config
    assert sut.config == expected_config_after

#
# def test_get_target_headers_base_directory_will_return_full_headers_base_directory_path():
#     expected_variable_name = '${BEAST_INCLUDE_DIR}'
#     expected_variable_value = 'variable_value'
#     expected_variables_map = {expected_variable_name: expected_variable_value}
#
#     file_path = "path/to/json/file/json"
#     test_data = CommonTestData()
#     test_data.target_cmake_variables_file_opener_mock.open = MagicMock(return_value=expected_variables_map)
#
#     test_data.json_manager_mock.load_from_file.return_value = test_data.config_json
#     sut = ConfigManager(file_path, test_data.json_manager_mock, test_data.target_cmake_variables_file_opener_mock)
#
#     sut.get_target_headers_base_directory(sut.config.cmake.lib)
