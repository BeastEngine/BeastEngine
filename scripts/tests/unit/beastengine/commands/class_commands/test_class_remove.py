import argparse
import builtins
import sys
from copy import deepcopy

import pytest
from mock import MagicMock, call
from src.config.config import Config, Config
from src.beastengine.commands.class_commands import class_remove, class_files_helper
from tests.tests_utilities.micro_mock import MicroMock


class CommonTestData:
    def __init__(self):
        self.class_files_helper_mock = MagicMock(class_files_helper.ClassFilesHelper)

        self.parser_mock = MagicMock(argparse.ArgumentParser)
        self.parser_mock.parse_args = MagicMock()
        self.project_dir = 'project/dir'

        self.headers_base_dir = 'headers/base/dir'
        self.sources_base_dir = 'sources/base/dir'

        self.config = Config()
        self.config.cmake = Config.CMake()
        self.config.cmake.lib = Config.CMake.Target()
        self.config.cmake.lib.headers = Config.CMake.Target.Files()
        self.config.cmake.lib.sources = Config.CMake.Target.Files()

        self.config_manager_mock = MagicMock(Config)
        self.config_manager_mock.config = self.config

        self.print_mock = MagicMock()
        self.original_print = builtins.print

    def mock_create_arguments_parser_function(self):
        class_remove.create_arguments_parser = MagicMock(return_value=self.parser_mock)
        pass

    def mock_print_function(self):
        builtins.print = self.print_mock
        pass


def test_constructor_will_retrieve_all_arguments_starting_from_fifth():
    test_data = CommonTestData()
    test_data.mock_create_arguments_parser_function()
    test_data.class_files_helper_mock.class_files_exist = MagicMock(return_value=False)

    sys.argv = ['arg1', 'arg2', 'arg3', 'arg4', 'arg5', 'arg6']
    expected_arguments = ['arg5', 'arg6']

    class_remove.ClassRemove(
        test_data.headers_base_dir,
        test_data.sources_base_dir,
        test_data.class_files_helper_mock,
        test_data.config.cmake.lib,
        test_data.config_manager_mock
    )
    test_data.parser_mock.parse_args.assert_called_with(expected_arguments)


def test_constructor_will_add_required_class_name_argument_to_parser():
    expected_argument_name = 'class_name'
    expected_argument_help = 'class to add'
    expected_argument_metavar = '<class_name>'

    test_data = CommonTestData()
    test_data.mock_create_arguments_parser_function()
    test_data.parser_mock.add_argument = MagicMock()

    test_data.class_files_helper_mock.class_files_exist = MagicMock(return_value=False)

    class_remove.ClassRemove(
        test_data.headers_base_dir,
        test_data.sources_base_dir,
        test_data.class_files_helper_mock,
        test_data.config.cmake.lib,
        test_data.config_manager_mock
    )
    test_data\
        .parser_mock\
        .add_argument\
        .assert_has_calls(
            [call(expected_argument_name, help=expected_argument_help, metavar=expected_argument_metavar)],
            any_order=True
        )


@pytest.mark.parametrize('expected_verbose', [True, False])
def test_constructor_will_check_if_verbose_option_is_set(expected_verbose):
    test_data = CommonTestData()
    test_data.mock_create_arguments_parser_function()

    test_data.class_files_helper_mock.class_files_exist = MagicMock(return_value=False)
    class_remove.is_verbose_set = MagicMock(return_value=expected_verbose)

    class_remove.ClassRemove(
        test_data.headers_base_dir,
        test_data.sources_base_dir,
        test_data.class_files_helper_mock,
        test_data.config.cmake.lib,
        test_data.config_manager_mock
    )

    class_remove.is_verbose_set.assert_called_once()


def test_constructor_will_check_if_files_corresponding_to_passed_class_name_already_exist():
    expected_class_name = 'class_name'

    test_data = CommonTestData()
    test_data.class_files_helper_mock.class_files_exist = MagicMock(return_value=False)

    cli_arguments_mock = MicroMock(class_name=expected_class_name)
    parser_mock = MicroMock(add_argument=MagicMock(), parse_args=MagicMock(return_value=cli_arguments_mock))
    class_remove.create_arguments_parser = MagicMock(return_value=parser_mock)

    class_remove.ClassRemove(
        test_data.headers_base_dir,
        test_data.sources_base_dir,
        test_data.class_files_helper_mock,
        test_data.config.cmake.lib,
        test_data.config_manager_mock
    )

    test_data.class_files_helper_mock.class_files_exist.assert_called_with(test_data.config.cmake.lib, expected_class_name)


def test_constructor_will_remove_class_files_if_they_exist():
    expected_class_name = 'class_name'
    expected_do_class_exist_return_value = True
    expected_is_verbose = True

    header_file_name = f'{expected_class_name}.h'
    source_file_name = f'{expected_class_name}.cpp'

    test_data = CommonTestData()
    test_data.class_files_helper_mock.class_files_exist = MagicMock(return_value=expected_do_class_exist_return_value)
    test_data.class_files_helper_mock.delete_class_files = MagicMock()
    test_data.class_files_helper_mock.get_header_file_name = MagicMock(return_value=header_file_name)
    test_data.class_files_helper_mock.get_source_file_name = MagicMock(return_value=source_file_name)

    config = deepcopy(test_data.config.cmake.lib)
    config.headers.files = [header_file_name]
    config.sources.files = [source_file_name]

    cli_arguments_mock = MicroMock(class_name=expected_class_name)
    parser_mock = MicroMock(add_argument=MagicMock(), parse_args=MagicMock(return_value=cli_arguments_mock))
    class_remove.create_arguments_parser = MagicMock(return_value=parser_mock)

    class_remove.is_verbose_set = MagicMock(return_value=expected_is_verbose)

    class_remove.ClassRemove(
        test_data.headers_base_dir,
        test_data.sources_base_dir,
        test_data.class_files_helper_mock,
        config,
        test_data.config_manager_mock
    )

    test_data\
        .class_files_helper_mock\
        .delete_class_files\
        .assert_called_with(
            expected_class_name,
            test_data.headers_base_dir,
            test_data.sources_base_dir,
            expected_is_verbose
        )


def test_constructor_will_not_remove_class_files_if_they_do_not_exist():
    expected_do_class_exist_return_value = False
    class_name = 'class_name'

    test_data = CommonTestData()
    test_data.class_files_helper_mock.class_files_exist = MagicMock(return_value=expected_do_class_exist_return_value)
    test_data.class_files_helper_mock.delete_class_files = MagicMock()

    cli_arguments_mock = MicroMock(class_name=class_name)
    parser_mock = MicroMock(add_argument=MagicMock(), parse_args=MagicMock(return_value=cli_arguments_mock))
    class_remove.create_arguments_parser = MagicMock(return_value=parser_mock)

    class_remove.ClassRemove(
        test_data.headers_base_dir,
        test_data.sources_base_dir,
        test_data.class_files_helper_mock,
        test_data.config.cmake.lib,
        test_data.config_manager_mock
    )

    test_data.class_files_helper_mock.delete_class_files.assert_not_called()


def test_constructor_will_remove_class_files_from_config():
    class_name = 'class_name'
    header_file_name = f'{class_name}.h'
    source_file_name = f'{class_name}.cpp'

    expected_header_files_after = []
    expected_source_files_after = []

    test_data = CommonTestData()
    test_data.class_files_helper_mock.class_files_exist = MagicMock(return_value=True)
    test_data.class_files_helper_mock.delete_class_files = MagicMock()
    test_data.class_files_helper_mock.get_header_file_name = MagicMock(return_value=header_file_name)
    test_data.class_files_helper_mock.get_source_file_name = MagicMock(return_value=source_file_name)

    cli_arguments_mock = MicroMock(class_name=class_name)
    parser_mock = MicroMock(add_argument=MagicMock(), parse_args=MagicMock(return_value=cli_arguments_mock))
    class_remove.create_arguments_parser = MagicMock(return_value=parser_mock)

    config = deepcopy(test_data.config.cmake.lib)
    config.headers.files = [header_file_name]
    config.sources.files = [source_file_name]

    class_remove.ClassRemove(
        test_data.headers_base_dir,
        test_data.sources_base_dir,
        test_data.class_files_helper_mock,
        config,
        test_data.config_manager_mock
    )

    assert config.headers.files == expected_header_files_after
    assert config.sources.files == expected_source_files_after


def test_constructor_will_update_config():
    class_name = 'class_name'
    header_file_name = f'{class_name}.h'
    source_file_name = f'{class_name}.cpp'

    test_data = CommonTestData()
    test_data.class_files_helper_mock.class_files_exist = MagicMock(return_value=True)
    test_data.class_files_helper_mock.delete_class_files = MagicMock()
    test_data.class_files_helper_mock.get_header_file_name = MagicMock(return_value=header_file_name)
    test_data.class_files_helper_mock.get_source_file_name = MagicMock(return_value=source_file_name)

    config = deepcopy(test_data.config.cmake.lib)
    config.headers.files = [header_file_name]
    config.sources.files = [source_file_name]

    cli_arguments_mock = MicroMock(class_name=class_name)
    parser_mock = MicroMock(add_argument=MagicMock(), parse_args=MagicMock(return_value=cli_arguments_mock))
    class_remove.create_arguments_parser = MagicMock(return_value=parser_mock)

    class_remove.ClassRemove(
        test_data.headers_base_dir,
        test_data.sources_base_dir,
        test_data.class_files_helper_mock,
        config,
        test_data.config_manager_mock
    )

    test_data.config_manager_mock.update.assert_called_once()
