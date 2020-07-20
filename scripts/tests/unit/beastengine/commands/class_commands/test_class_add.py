import argparse
import builtins
import sys
from copy import deepcopy

import pytest
from mock import MagicMock, call
from src.config.config_manager import ConfigManager, Config
from src.beastengine.commands.class_commands import class_add, class_files_helper
from tests.tests_utilities.micro_mock import MicroMock


class CommonTestData:
    def __init__(self):
        self.argv = ['arg1', 'arg2', 'arg3', 'arg4']

        self.class_files_helper_mock = MagicMock(class_files_helper.ClassFilesHelper)

        self.parser_mock = MagicMock(argparse.ArgumentParser)
        self.parser_mock.parse_args = MagicMock()
        self.exclusive_group_mock = MagicMock()
        self.exclusive_group_mock.add_argument = MagicMock()
        self.mutually_exclusive_group_mock = MagicMock(return_value=self.exclusive_group_mock)

        self.project_dir = 'project/dir'

        self.headers_base_dir = 'headers/base/dir'
        self.sources_base_dir = 'sources/base/dir'

        self.config = Config()
        self.config.cmake = Config.CMake()
        self.config.cmake.lib = Config.CMake.Target()
        self.config.cmake.lib.headers = Config.CMake.Target.Files()
        self.config.cmake.lib.sources = Config.CMake.Target.Files()

        self.config_manager_mock = MagicMock(ConfigManager)
        self.config_manager_mock.config = self.config

        self.print_mock = MagicMock()
        self.original_print = builtins.print

    def mock_create_arguments_parser_function(self):
        class_add.create_arguments_parser = MagicMock(return_value=self.parser_mock)
        pass

    def mock_print_function(self):
        builtins.print = self.print_mock
        pass

    def get_cli_arguments_mock(self, class_name, namespace=None, header_only=None, source_only=None):
        return MicroMock(class_name=class_name, namespace=namespace, header_only=header_only, source_only=source_only)


def test_constructor_will_retrieve_all_arguments_starting_from_fourth():
    test_data = CommonTestData()
    test_data.mock_create_arguments_parser_function()
    test_data.class_files_helper_mock.class_files_exist = MagicMock(return_value=False)

    sys.argv = ['arg1', 'arg2', 'arg3', 'arg4']
    expected_arguments = ['arg4']

    class_add.ClassAdd(
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

    class_add.ClassAdd(
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


def test_constructor_will_add_namespace_optional_argument_to_parser():
    expected_argument_name_short = '-n'
    expected_argument_full_name = '--namespace'
    expected_argument_help = 'namespace in which the class should reside'
    expected_argument_type = str

    test_data = CommonTestData()
    test_data.mock_create_arguments_parser_function()
    test_data.parser_mock.add_argument = MagicMock()

    test_data.class_files_helper_mock.class_files_exist = MagicMock(return_value=False)

    class_add.ClassAdd(
        test_data.headers_base_dir,
        test_data.sources_base_dir,
        test_data.class_files_helper_mock,
        test_data.config.cmake.lib,
        test_data.config_manager_mock
    )
    test_data \
        .parser_mock \
        .add_argument \
        .assert_has_calls(
            [call(expected_argument_name_short, expected_argument_full_name, help=expected_argument_help, type=expected_argument_type)],
            any_order=True
        )


@pytest.mark.parametrize('expected_verbose', [True, False])
def test_constructor_will_check_if_verbose_option_is_set(expected_verbose):
    test_data = CommonTestData()
    test_data.mock_create_arguments_parser_function()

    test_data.class_files_helper_mock.class_files_exist = MagicMock(return_value=False)
    class_add.is_verbose_set = MagicMock(return_value=expected_verbose)

    class_add.ClassAdd(
        test_data.headers_base_dir,
        test_data.sources_base_dir,
        test_data.class_files_helper_mock,
        test_data.config.cmake.lib,
        test_data.config_manager_mock
    )

    class_add.is_verbose_set.assert_called_once()


def test_constructor_will_check_if_files_corresponding_to_passed_class_name_already_exist():
    expected_class_name = 'class_name'
    test_data = CommonTestData()
    test_data.class_files_helper_mock.class_files_exist = MagicMock(return_value=False)

    cli_arguments_mock = test_data.get_cli_arguments_mock(expected_class_name)
    parser_mock = MicroMock(add_argument=MagicMock(), parse_args=MagicMock(return_value=cli_arguments_mock))
    parser_mock.add_mutually_exclusive_group = test_data.mutually_exclusive_group_mock
    class_add.create_arguments_parser = MagicMock(return_value=parser_mock)

    class_add.ClassAdd(
        test_data.headers_base_dir,
        test_data.sources_base_dir,
        test_data.class_files_helper_mock,
        test_data.config.cmake.lib,
        test_data.config_manager_mock
    )

    test_data.class_files_helper_mock.class_files_exist.assert_called_with(test_data.config.cmake.lib, expected_class_name)


def test_constructor_will_print_error_message_if_class_already_exists():
    class_name = 'class_name'
    test_data = CommonTestData()
    test_data.mock_print_function()

    expected_error_message = f"'{class_name}' class already exists!"

    cli_arguments_mock = test_data.get_cli_arguments_mock(class_name)
    parser_mock = MicroMock(add_argument=MagicMock(), parse_args=MagicMock(return_value=cli_arguments_mock))
    parser_mock.add_mutually_exclusive_group = test_data.mutually_exclusive_group_mock
    class_add.create_arguments_parser = MagicMock(return_value=parser_mock)

    test_data.class_files_helper_mock.class_files_exist = MagicMock(return_value=True)

    class_add.ClassAdd(
        test_data.headers_base_dir,
        test_data.sources_base_dir,
        test_data.class_files_helper_mock,
        test_data.config.cmake.lib,
        test_data.config_manager_mock
    )

    test_data.print_mock.assert_called_with(expected_error_message)


def test_constructor_will_create_class_if_passed_class_does_not_exist():
    expected_class_name = 'class_name'
    expected_namespace = None

    test_data = CommonTestData()
    test_data.class_files_helper_mock.create_class_files = MagicMock()
    is_verbose = False

    cli_arguments_mock = test_data.get_cli_arguments_mock(expected_class_name, expected_namespace)
    parser_mock = MicroMock(add_argument=MagicMock(), parse_args=MagicMock(return_value=cli_arguments_mock))
    parser_mock.add_mutually_exclusive_group = test_data.mutually_exclusive_group_mock
    class_add.create_arguments_parser = MagicMock(return_value=parser_mock)

    test_data.class_files_helper_mock.class_files_exist = MagicMock(return_value=False)
    class_add.is_verbose_set = MagicMock(return_value=is_verbose)

    class_add.ClassAdd(
        test_data.headers_base_dir,
        test_data.sources_base_dir,
        test_data.class_files_helper_mock,
        test_data.config.cmake.lib,
        test_data.config_manager_mock
    )

    test_data\
        .class_files_helper_mock\
        .create_class_files\
        .assert_called_with(
            expected_class_name,
            test_data.headers_base_dir,
            test_data.sources_base_dir,
            is_verbose,
            expected_namespace
        )


def test_constructor_will_create_class_with_namespace_parameter_if_one_specified():
    expected_namespace = 'test::namespace'
    class_name = 'class_name'
    is_verbose = False

    test_data = CommonTestData()
    test_data.class_files_helper_mock.create_class_files = MagicMock()

    cli_arguments_mock = test_data.get_cli_arguments_mock(class_name, expected_namespace)
    parser_mock = MicroMock(add_argument=MagicMock(), parse_args=MagicMock(return_value=cli_arguments_mock))
    parser_mock.add_mutually_exclusive_group = test_data.mutually_exclusive_group_mock
    class_add.create_arguments_parser = MagicMock(return_value=parser_mock)

    test_data.class_files_helper_mock.class_files_exist = MagicMock(return_value=False)
    class_add.is_verbose_set = MagicMock(return_value=is_verbose)

    class_add.ClassAdd(
        test_data.headers_base_dir,
        test_data.sources_base_dir,
        test_data.class_files_helper_mock,
        test_data.config.cmake.lib,
        test_data.config_manager_mock
    )

    test_data\
        .class_files_helper_mock\
        .create_class_files\
        .assert_called_with(
            class_name,
            test_data.headers_base_dir,
            test_data.sources_base_dir,
            is_verbose,
            expected_namespace
        )


def test_constructor_will_add_files_names_to_given_target_files_config():
    expected_class_name = 'class_name'
    expected_header_file_name = f'{expected_class_name}.h'
    expected_source_file_name = f'{expected_class_name}.cpp'
    expected_header_files = [expected_header_file_name]
    expected_source_files = [expected_source_file_name]

    test_data = CommonTestData()
    config = deepcopy(test_data.config.cmake.lib)
    config.headers = Config.CMake.Target.Files()
    config.headers.files = []

    config.sources = Config.CMake.Target.Files()
    config.sources.files = []

    test_data.class_files_helper_mock.class_files_exist = MagicMock(return_value=False)
    test_data.class_files_helper_mock.get_header_file_name = MagicMock(return_value=expected_header_file_name)
    test_data.class_files_helper_mock.get_source_file_name = MagicMock(return_value=expected_source_file_name)

    cli_arguments_mock = test_data.get_cli_arguments_mock(expected_class_name)
    parser_mock = MicroMock(add_argument=MagicMock(), parse_args=MagicMock(return_value=cli_arguments_mock))
    parser_mock.add_mutually_exclusive_group = test_data.mutually_exclusive_group_mock
    class_add.create_arguments_parser = MagicMock(return_value=parser_mock)

    class_add.ClassAdd(
        test_data.headers_base_dir,
        test_data.sources_base_dir,
        test_data.class_files_helper_mock,
        config,
        test_data.config_manager_mock
    )

    assert config.headers.files == expected_header_files
    assert config.sources.files == expected_source_files


def test_constructor_will_add_files_names_to_given_target_files_config_without_affecting_already_defined_files():
    test_data = CommonTestData()
    config = deepcopy(test_data.config.cmake.lib)
    config.headers = Config.CMake.Target.Files()
    config.headers.files = ['some_class_before1.h', 'some_class_before2.h']

    config.sources = Config.CMake.Target.Files()
    config.sources.files = ['some_class_before1.cpp', 'some_class_before2.cpp']

    expected_class_name = 'class_name'
    expected_header_file_name = f'{expected_class_name}.h'
    expected_source_file_name = f'{expected_class_name}.cpp'
    expected_header_files = ['some_class_before1.h', 'some_class_before2.h', expected_header_file_name]
    expected_source_files = ['some_class_before1.cpp', 'some_class_before2.cpp', expected_source_file_name]

    test_data.class_files_helper_mock.class_files_exist = MagicMock(return_value=False)
    test_data.class_files_helper_mock.get_header_file_name = MagicMock(return_value=expected_header_file_name)
    test_data.class_files_helper_mock.get_source_file_name = MagicMock(return_value=expected_source_file_name)

    cli_arguments_mock = test_data.get_cli_arguments_mock(expected_class_name)
    parser_mock = MicroMock(add_argument=MagicMock(), parse_args=MagicMock(return_value=cli_arguments_mock))
    parser_mock.add_mutually_exclusive_group = test_data.mutually_exclusive_group_mock
    class_add.create_arguments_parser = MagicMock(return_value=parser_mock)

    class_add.ClassAdd(
        test_data.headers_base_dir,
        test_data.sources_base_dir,
        test_data.class_files_helper_mock,
        config,
        test_data.config_manager_mock
    )

    assert config.headers.files == expected_header_files
    assert config.sources.files == expected_source_files


def test_constructor_will_update_config_after_class_is_added():
    test_data = CommonTestData()

    class_name = 'class_name'
    header_file_name = f'{class_name}.h'
    source_file_name = f'{class_name}.cpp'

    test_data.class_files_helper_mock.class_files_exist = MagicMock(return_value=False)
    test_data.class_files_helper_mock.get_header_file_name = MagicMock(return_value=header_file_name)
    test_data.class_files_helper_mock.get_source_file_name = MagicMock(return_value=source_file_name)

    cli_arguments_mock = test_data.get_cli_arguments_mock(class_name)
    parser_mock = MicroMock(add_argument=MagicMock(), parse_args=MagicMock(return_value=cli_arguments_mock))
    parser_mock.add_mutually_exclusive_group = test_data.mutually_exclusive_group_mock
    class_add.create_arguments_parser = MagicMock(return_value=parser_mock)

    class_add.ClassAdd(
        test_data.headers_base_dir,
        test_data.sources_base_dir,
        test_data.class_files_helper_mock,
        test_data.config.cmake.lib,
        test_data.config_manager_mock
    )

    test_data.config_manager_mock.update_config.assert_called_once()
