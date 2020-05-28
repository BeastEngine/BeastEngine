import builtins
import subprocess
import colorama

from unittest.mock import MagicMock, Mock

from mock import call

from src.commandrunners.command_runner import CommandRunner


class SubprocessMock:
    RETURN_CODE_SUCCESS = 0

    def __init__(self, expected_return_code=None, expected_stdout=None, expected_stderr=None):
        process_result_mock = MagicMock(subprocess.CompletedProcess)
        type(process_result_mock).returncode = expected_return_code

        type(process_result_mock).stdout = expected_stdout
        type(process_result_mock).stderr = expected_stderr

        self.run_func_mock = MagicMock(return_value=process_result_mock)
        subprocess.run = self.run_func_mock


def get_command_to_print(command):
    return CommandRunner.RUNNING_COMMAND_PRINT_TEMPLATE.format(
        color=CommandRunner.RUNNING_COMMAND_PRINT_COLOR,
        command=command
    )


def test_run_command_will_start_subprocess_with_given_command():
    subprocess_mock = SubprocessMock()

    expected_command = 'ls -l'
    expected_capture_output_param = True
    expected_text_param = True
    cwd = 'cwd'

    sut = CommandRunner()
    sut.run_command(expected_command, cwd)

    subprocess_mock.run_func_mock.assert_called_with(
        args=expected_command,
        capture_output=expected_capture_output_param,
        text=expected_text_param,
        cwd=cwd
    )


def test_run_command_will_return_subprocess_return_code():
    expected_return_code = 0
    cwd = 'cwd'

    process_result_mock = MagicMock(subprocess.CompletedProcess)
    type(process_result_mock).returncode = expected_return_code

    subprocess_run_mock = MagicMock(return_value=process_result_mock)
    subprocess.run = subprocess_run_mock

    sut = CommandRunner()
    actual_return_code = sut.run_command('ls -l', cwd)

    assert expected_return_code == actual_return_code


def test_run_command_will_print_name_of_the_executed_command_in_right_color_if_verbose_flag_passed():
    SubprocessMock()

    cwd = 'cwd'
    command_to_execute = 'ls -l'
    expected_command_print = get_command_to_print(command_to_execute)

    print_mock = MagicMock()
    builtins.print = print_mock

    sut = CommandRunner()
    sut.run_command(command_to_execute, cwd, verbose=True)

    print_mock.assert_called_with(expected_command_print)


def test_run_command_will_print_stdout_in_right_color_if_verbose_flag_passed_and_result_contains_stdout():
    cwd = 'cwd'

    command_to_execute = 'ls -l'
    command_to_print = get_command_to_print(command_to_execute)
    print_command_call = call(command_to_print)

    subprocess_stdout = "subprocess stdout"
    SubprocessMock(expected_stdout=subprocess_stdout)

    expected_stdout = colorama.Fore.LIGHTWHITE_EX + subprocess_stdout
    expected_stdout_print_call = call(expected_stdout)

    print_mock = MagicMock()
    builtins.print = print_mock

    sut = CommandRunner()
    sut.run_command(command_to_execute, cwd, verbose=True)

    print_mock.assert_has_calls([print_command_call, expected_stdout_print_call])


def test_run_command_will_not_print_stdout_in_right_color_if_verbose_flag_passed_and_result_does_not_contain_stdout():
    cwd = 'cwd'

    command_to_execute = 'ls -l'
    command_to_print = get_command_to_print(command_to_execute)

    subprocess_stdout = None
    SubprocessMock(expected_stdout=subprocess_stdout)

    print_mock = MagicMock()
    builtins.print = print_mock

    sut = CommandRunner()
    sut.run_command(command_to_execute, cwd, verbose=True)

    print_mock.assert_called_once_with(command_to_print)


def test_run_command_will_not_print_stdout_in_right_color_if_verbose_flag_passed_and_result_contains_empty_stdout():
    cwd = 'cwd'

    command_to_execute = 'ls -l'
    command_to_print = get_command_to_print(command_to_execute)

    subprocess_stdout = ""
    SubprocessMock(expected_stdout=subprocess_stdout)

    print_mock = MagicMock()
    builtins.print = print_mock

    sut = CommandRunner()
    sut.run_command(command_to_execute, cwd, verbose=True)

    print_mock.assert_called_once_with(command_to_print)


def test_run_command_will_print_stderr_of_run_subprocess_in_right_color_if_verbose_flag_passed():
    cwd = 'cwd'

    command_to_execute = 'ls -l'
    command_to_print = get_command_to_print(command_to_execute)
    print_command_call = call(command_to_print)

    subprocess_stderr = "subprocess stderr"
    SubprocessMock(expected_stderr=subprocess_stderr)

    expected_stdout = colorama.Fore.LIGHTRED_EX + subprocess_stderr
    expected_stout_print_call = call(expected_stdout)

    print_mock = MagicMock()
    builtins.print = print_mock

    sut = CommandRunner()
    sut.run_command(command_to_execute, cwd, verbose=True)

    print_mock.assert_has_calls([print_command_call, expected_stout_print_call])


def test_run_command_will_not_print_stderr_in_right_color_if_verbose_flag_passed_and_result_does_not_contain_stderr():
    cwd = 'cwd'

    command_to_execute = 'ls -l'
    command_to_print = get_command_to_print(command_to_execute)

    subprocess_stderr = None
    SubprocessMock(expected_stderr=subprocess_stderr)

    print_mock = MagicMock()
    builtins.print = print_mock

    sut = CommandRunner()
    sut.run_command(command_to_execute, cwd, verbose=True)

    print_mock.assert_called_once_with(command_to_print)


def test_run_command_will_not_print_stderr_in_right_color_if_verbose_flag_passed_and_result_contains_empty_stderr():
    cwd = 'cwd'

    command_to_execute = 'ls -l'
    command_to_print = get_command_to_print(command_to_execute)

    subprocess_stderr = ""
    SubprocessMock(expected_stderr=subprocess_stderr)

    print_mock = MagicMock()
    builtins.print = print_mock

    sut = CommandRunner()
    sut.run_command(command_to_execute, cwd, verbose=True)

    print_mock.assert_called_once_with(command_to_print)


def test_run_command_will_not_print_anything_if_verbose_is_false():
    cwd = 'cwd'

    subprocess_stdout = "subprocess stdout"
    subprocess_stderr = "subprocess stderr"
    SubprocessMock(expected_stdout=subprocess_stdout, expected_stderr=subprocess_stderr)

    print_mock = MagicMock()
    builtins.print = print_mock

    sut = CommandRunner()
    sut.run_command('ls -l', cwd, verbose=False)

    print_mock.assert_not_called()


def test_run_command_will_not_print_anything_if_verbose_parameter_is_omitted():
    cwd = 'cwd'

    subprocess_stdout = "subprocess stdout"
    subprocess_stderr = "subprocess stderr"
    SubprocessMock(expected_stdout=subprocess_stdout, expected_stderr=subprocess_stderr)

    print_mock = MagicMock()
    builtins.print = print_mock

    sut = CommandRunner()
    sut.run_command('ls -l', cwd)

    print_mock.assert_not_called()


def test_run_command_will_execute_command_from_passed_working_directory():
    subprocess_mock = SubprocessMock()

    expected_current_working_dir = 'current/working/dir'
    command = 'ls -l'
    capture_output_param = True
    text_param = True

    sut = CommandRunner()
    sut.run_command(command, expected_current_working_dir)
    subprocess_mock.run_func_mock.assert_called_with(
        args=command,
        capture_output=capture_output_param,
        text=text_param,
        cwd=expected_current_working_dir
    )

