import argparse
import src.functions as functions

from mock import MagicMock

from tests.tests_utilities.micro_mock import MicroMock


def test_get_project_path():
    expected_path = "path/to/project"

    traceback_mock = MicroMock(filename='filename')
    functions.getframeinfo = MagicMock(return_value=traceback_mock)

    path_mock = MicroMock(parent=MicroMock(parent=MicroMock(parent=expected_path)))
    functions.Path.resolve = MagicMock(return_value=path_mock)

    assert functions.get_project_path() == expected_path


def test_get_build_dir_name_will_return_valid_build_dir_name():
    expected_dir_name = "build"
    assert functions.get_build_dir_name() == expected_dir_name


def test_get_build_dir_path_will_return_valid_path():
    build_dir_name = "build"
    project_path = "project/path"
    functions.get_project_path = MagicMock(return_value=project_path)

    expected_build_dir_path = f"{project_path}/{build_dir_name}"
    assert functions.get_build_dir_path() == expected_build_dir_path


def test_get_config_path_will_return_valid_path():
    config_path = "config/config.json"
    project_path = "project/path"
    functions.get_project_path = MagicMock(return_value=project_path)

    expected_config_path = f"{project_path}/{config_path}"
    assert functions.get_config_path() == expected_config_path


def test_is_verbose_set_will_return_false_when_verbose_attribute_is_set_to_false():
    data = MicroMock(verbose=False)

    expected_result = False
    assert functions.is_verbose_set(data) == expected_result


def test_is_verbose_set_will_return_false_when_verbose_attribute_is_set_to_true():
    data = MicroMock(verbose=True)

    expected_result = True
    assert functions.is_verbose_set(data) == expected_result


def test_create_arguments_parser_will_create_empty_parser_when_no_data_passed():
    expected_program = None
    expected_usage = None
    expected_description = None

    argument_parser_mock = MagicMock(argparse.ArgumentParser)
    functions.ArgumentParser = argument_parser_mock

    functions.create_arguments_parser()
    argument_parser_mock.assert_called_with(
        prog=expected_program,
        usage=expected_usage,
        description=expected_description,
    )


def test_create_arguments_parser_will_create_parser_with_given_program():
    expected_program = 'Program'

    argument_parser_mock = MagicMock(argparse.ArgumentParser)
    functions.ArgumentParser = argument_parser_mock

    functions.create_arguments_parser(program=expected_program)
    argument_parser_mock.assert_called_with(
        prog=expected_program,
        usage=None,
        description=None,
    )


def test_create_arguments_parser_will_create_parser_with_given_usage():
    expected_usage = 'Program Usage'

    argument_parser_mock = MagicMock(argparse.ArgumentParser)
    functions.ArgumentParser = argument_parser_mock

    functions.create_arguments_parser(usage=expected_usage)
    argument_parser_mock.assert_called_with(
        prog=None,
        usage=expected_usage,
        description=None,
    )


def test_create_arguments_parser_will_create_parser_with_given_description():
    expected_description = 'Program Description'

    argument_parser_mock = MagicMock(argparse.ArgumentParser)
    functions.ArgumentParser = argument_parser_mock

    functions.create_arguments_parser(description=expected_description)
    argument_parser_mock.assert_called_with(
        prog=None,
        usage=None,
        description=expected_description,
    )


def test_create_arguments_parser_will_create_parser_with_given_formatter_class():
    expected_formatter = MagicMock()

    argument_parser_mock = MagicMock(argparse.ArgumentParser)
    functions.ArgumentParser = argument_parser_mock

    functions.create_arguments_parser(formatter_class=expected_formatter)
    argument_parser_mock.assert_called_with(
        prog=None,
        usage=None,
        description=None,
        formatter_class=expected_formatter
    )


def test_create_arguments_parser_will_create_parser_with_verbose_as_optional_argument():
    expected_argument_short_name = '-v'
    expected_argument_full_name = '--verbose'
    expected_argument_help = "show command output"
    expected_argument_action = "store_true"

    add_argument_mock = MagicMock()
    functions.ArgumentParser = argparse.ArgumentParser
    functions.ArgumentParser.add_argument = add_argument_mock

    functions.create_arguments_parser()
    add_argument_mock.assert_called_with(
        expected_argument_short_name,
        expected_argument_full_name,
        help=expected_argument_help,
        action=expected_argument_action
    )
