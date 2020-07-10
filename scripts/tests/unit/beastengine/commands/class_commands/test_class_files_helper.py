from mock import MagicMock, call

from src.files.file_opener import FileOpener
from src.commandrunners.command_runner import CommandRunner
from src.config.config_manager import Config
from src.beastengine.commands.class_commands import class_files_helper


def test_class_files_exist_will_return_true_if_files_that_correspond_to_given_class_name_exist():
    file_mock = MagicMock(FileOpener.File)
    file_opener_mock = MagicMock(FileOpener)
    file_opener_mock.open = MagicMock(return_value=file_mock)

    command_runner_mock = MagicMock(CommandRunner)
    class_name = 'test_class'

    target_config = Config().CMake.Target()
    target_config.headers = Config.CMake.Target.Files()
    target_config.headers.files = [f'{class_name}.h']

    target_config.sources = Config.CMake.Target.Files()
    target_config.sources.files = [f'{class_name}.cpp']

    sut = class_files_helper.ClassFilesHelper(command_runner_mock, file_opener_mock)
    assert sut.class_files_exist(target_config, class_name) is True


def test_class_files_exist_will_return_false_if_files_that_correspond_to_given_class_name_exist():
    file_mock = MagicMock(FileOpener.File)
    file_opener_mock = MagicMock(FileOpener)
    file_opener_mock.open = MagicMock(return_value=file_mock)

    command_runner_mock = MagicMock(CommandRunner)
    non_existent_class_name = 'non_existent_class'

    target_config = Config().CMake.Target()
    target_config.headers = Config.CMake.Target.Files()
    target_config.headers.files = ['some_class.h']

    target_config.sources = Config.CMake.Target.Files()
    target_config.sources.files = ['some_class.cpp']

    sut = class_files_helper.ClassFilesHelper(command_runner_mock, file_opener_mock)
    assert sut.class_files_exist(target_config, non_existent_class_name) is False


def test_create_class_files_will_create_class_files():
    file_mock = MagicMock(FileOpener.File)
    file_opener_mock = MagicMock(FileOpener)
    file_opener_mock.open = MagicMock(return_value=file_mock)

    command_runner_mock = MagicMock(CommandRunner)
    command_runner_mock.run_command = MagicMock()

    class_name = 'test_class'
    headers_base_dir = '/headers/base/directory/path'
    sources_base_dir = '/sources/base/directory/path'
    is_verbose = True

    expected_header_file_path = f'{headers_base_dir}/{class_name}.h'
    expected_sources_file_path = f'{sources_base_dir}/{class_name}.cpp'
    expected_cwd = '/project/dir'

    expected_create_header_file_call = call(f'touch {expected_header_file_path}', expected_cwd, is_verbose)
    expected_create_source_file_call = call(f'touch {expected_sources_file_path}', expected_cwd, is_verbose)

    class_files_helper.get_project_path = MagicMock(return_value=expected_cwd)

    sut = class_files_helper.ClassFilesHelper(command_runner_mock, file_opener_mock)
    sut.create_class_files(class_name, headers_base_dir, sources_base_dir, is_verbose)

    command_runner_mock.run_command.assert_has_calls([expected_create_header_file_call, expected_create_source_file_call])


def test_create_class_files_will_create_proper_directories_if_class_name_contains_directories():
    file_mock = MagicMock(FileOpener.File)
    file_opener_mock = MagicMock(FileOpener)
    file_opener_mock.open = MagicMock(return_value=file_mock)

    command_runner_mock = MagicMock(CommandRunner)
    command_runner_mock.run_command = MagicMock()

    class_sub_directory = 'class/sub/directory'
    class_name = f'{class_sub_directory}/test_class'

    headers_base_dir = '/headers/base/directory/path'
    sources_base_dir = '/sources/base/directory/path'
    project_directory = '/project/dir'
    is_verbose = True

    expected_header_file_directory_creation_call = call(f'mkdir {class_sub_directory}', headers_base_dir, is_verbose)
    expected_source_file_directory_creation_call = call(f'mkdir {class_sub_directory}', sources_base_dir, is_verbose)

    class_files_helper.get_project_path = MagicMock(return_value=project_directory)

    sut = class_files_helper.ClassFilesHelper(command_runner_mock, file_opener_mock)
    sut.create_class_files(class_name, headers_base_dir, sources_base_dir, is_verbose)

    command_runner_mock\
        .run_command\
        .assert_has_calls(
            [
                expected_header_file_directory_creation_call,
                expected_source_file_directory_creation_call
            ],
            any_order=True
        )


def test_create_class_files_will_not_create_sub_directories_if_class_name_contains_directories_and_directories_already_exist():
    file_mock = MagicMock(FileOpener.File)
    file_opener_mock = MagicMock(FileOpener)
    file_opener_mock.open = MagicMock(return_value=file_mock)

    command_runner_mock = MagicMock(CommandRunner)
    command_runner_mock.run_command = MagicMock()

    class_sub_directory = 'class/sub/directory'
    class_name = f'{class_sub_directory}/test_class'

    headers_base_dir = '/headers/base/directory/path'
    sources_base_dir = '/sources/base/directory/path'
    project_directory = '/project/dir'
    is_verbose = True

    header_file_path = f'{headers_base_dir}/{class_name}.h'
    sources_file_path = f'{sources_base_dir}/{class_name}.cpp'
    expected_create_header_file_call = call(f'touch {header_file_path}', project_directory, is_verbose)
    expected_create_source_file_call = call(f'touch {sources_file_path}', project_directory, is_verbose)

    expected_header_file_directories_existence_check_call = call(f'{headers_base_dir}/{class_sub_directory}')
    expected_source_file_directories_existence_check_call = call(f'{sources_base_dir}/{class_sub_directory}')

    class_files_helper.get_project_path = MagicMock(return_value=project_directory)

    path_mock = MagicMock()
    path_mock.exists = MagicMock(return_value=True)
    class_files_helper.Path = MagicMock(return_value=path_mock)

    sut = class_files_helper.ClassFilesHelper(command_runner_mock, file_opener_mock)
    sut.create_class_files(class_name, headers_base_dir, sources_base_dir, is_verbose)

    class_files_helper.Path.assert_has_calls(
        [expected_header_file_directories_existence_check_call, expected_source_file_directories_existence_check_call]
    )
    command_runner_mock.run_command.assert_has_calls([expected_create_header_file_call, expected_create_source_file_call])

    path_mock.exists.assert_called_once()
    assert command_runner_mock.run_command.call_count == 2


def test_create_class_files_will_create_class_files_with_proper_content():
    file_mock = MagicMock(FileOpener.File)
    file_opener_mock = MagicMock(FileOpener)
    file_opener_mock.open = MagicMock(return_value=file_mock)

    command_runner_mock = MagicMock(CommandRunner)
    command_runner_mock.run_command = MagicMock()

    class_name = 'test_class'
    headers_base_dir = '/headers/base/directory/path'
    sources_base_dir = '/sources/base/directory/path'
    is_verbose = True

    expected_header_file_content = '#pragma once'
    expected_source_file_content = f'#include "BeastEngine/{class_name}.h"'

    cwd = '/project/dir'
    class_files_helper.get_project_path = MagicMock(return_value=cwd)

    sut = class_files_helper.ClassFilesHelper(command_runner_mock, file_opener_mock)
    sut.create_class_files(class_name, headers_base_dir, sources_base_dir, is_verbose)

    file_mock.replace_content.assert_has_calls(
        [call(expected_header_file_content), call(expected_source_file_content)]
    )


def test_create_class_files_will_create_class_files_containing_namespace_definition_if_namespaces_parameter_passed():
    file_mock = MagicMock(FileOpener.File)
    file_opener_mock = MagicMock(FileOpener)
    file_opener_mock.open = MagicMock(return_value=file_mock)

    command_runner_mock = MagicMock(CommandRunner)
    command_runner_mock.run_command = MagicMock()

    class_name = 'test_class'
    headers_base_dir = '/headers/base/directory/path'
    sources_base_dir = '/sources/base/directory/path'
    is_verbose = True

    expected_namespace = 'BeastEngine'
    expected_header_file_content =\
f'''#pragma once

namespace {expected_namespace}
{{

}}
'''
    expected_source_file_content =\
f'''#include "BeastEngine/{class_name}.h"

namespace {expected_namespace}
{{

}}
'''

    cwd = '/project/dir'
    class_files_helper.get_project_path = MagicMock(return_value=cwd)

    sut = class_files_helper.ClassFilesHelper(command_runner_mock, file_opener_mock)
    sut.create_class_files(class_name, headers_base_dir, sources_base_dir, is_verbose, namespace=expected_namespace)

    file_mock.replace_content.assert_has_calls(
        [call(expected_header_file_content), call(expected_source_file_content)]
    )
