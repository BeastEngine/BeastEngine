import os
from pathlib import Path
from unittest.mock import MagicMock

from src.files.file_opener import FileOpener
from src.commandrunners.command_runner import CommandRunner
from src.beastengine.commands.class_commands.class_files_helper import ClassFilesHelper


def test_delete_class_files_will_remove_header_file(tmpdir):
    is_verbose = False
    class_name = 'class'

    headers_base_directory = f'{tmpdir}/headers'
    sources_base_directory = f'{tmpdir}/sources'

    header_file_name = f'{class_name}.h'
    header_file_path = f'{headers_base_directory}/{header_file_name}'

    os.mkdir(headers_base_directory)
    os.mkdir(sources_base_directory)
    open(header_file_path, 'x')

    command_runner = CommandRunner()
    file_opener_mock = MagicMock(FileOpener)

    sut = ClassFilesHelper(command_runner, file_opener_mock)
    sut.delete_class_files(class_name, headers_base_directory, sources_base_directory, is_verbose)

    header_file_path_after = Path(header_file_path)
    assert header_file_path_after.exists() is False


def test_delete_class_files_will_remove_header_file_and_its_subdirectory_when_subdirectory_is_empty(tmpdir):
    is_verbose = False
    class_subdirectory = 'subdirectory'
    class_name = f'{class_subdirectory}/class'

    headers_base_directory = f'{tmpdir}/headers'
    sources_base_directory = f'{tmpdir}/sources'

    header_file_name = f'{class_name}.h'
    header_file_path = f'{headers_base_directory}/{header_file_name}'
    header_file_subdirectory_path = f'{headers_base_directory}/{class_subdirectory}'

    os.mkdir(headers_base_directory)
    os.mkdir(sources_base_directory)

    os.mkdir(header_file_subdirectory_path)
    open(header_file_path, 'x')

    command_runner = CommandRunner()
    file_opener_mock = MagicMock(FileOpener)

    sut = ClassFilesHelper(command_runner, file_opener_mock)
    sut.delete_class_files(class_name, headers_base_directory, sources_base_directory, is_verbose)

    header_file_subdirectory_path_after = Path(header_file_subdirectory_path)
    assert header_file_subdirectory_path_after.exists() is False


def test_delete_class_files_will_remove_header_file_and_its_subdirectories_when_subdirectories_are_empty(tmpdir):
    is_verbose = False
    class_subdirectories = 'subdirectory1/subdirectory2'
    class_name = f'{class_subdirectories}/class'

    headers_base_directory = f'{tmpdir}/headers'
    sources_base_directory = f'{tmpdir}/sources'

    header_file_name = f'{class_name}.h'
    header_file_path = f'{headers_base_directory}/{header_file_name}'
    header_file_subdirectories_path = f'{headers_base_directory}/{class_subdirectories}'

    os.mkdir(headers_base_directory)
    os.mkdir(sources_base_directory)

    os.makedirs(header_file_subdirectories_path)
    open(header_file_path, 'x')

    command_runner = CommandRunner()
    file_opener_mock = MagicMock(FileOpener)

    sut = ClassFilesHelper(command_runner, file_opener_mock)
    sut.delete_class_files(class_name, headers_base_directory, sources_base_directory, is_verbose)

    assert not os.listdir(headers_base_directory)


def test_delete_class_files_will_remove_header_file_and_its_subdirectories_but_only_empty_ones(tmpdir):
    is_verbose = False
    not_empty_subdirectory = 'not_empty_subdirectory'
    additional_file = f'{not_empty_subdirectory}/additional_file.h'

    empty_subdirectory = 'empty_subdirectory'
    class_subdirectories = f'{not_empty_subdirectory}/{empty_subdirectory}'
    class_name = f'{class_subdirectories}/class'

    headers_base_directory = f'{tmpdir}/headers'
    sources_base_directory = f'{tmpdir}/sources'

    header_file_name = f'{class_name}.h'
    header_file_path = f'{headers_base_directory}/{header_file_name}'
    header_file_subdirectories_path = f'{headers_base_directory}/{class_subdirectories}'

    not_empty_subdirectory_path = f'{headers_base_directory}/{not_empty_subdirectory}'
    empty_subdirectory_path = f'{headers_base_directory}/{empty_subdirectory}'

    os.mkdir(headers_base_directory)
    os.mkdir(sources_base_directory)

    os.makedirs(header_file_subdirectories_path)
    open(f'{headers_base_directory}/{additional_file}', 'x')
    open(header_file_path, 'x')

    command_runner = CommandRunner()
    file_opener_mock = MagicMock(FileOpener)

    sut = ClassFilesHelper(command_runner, file_opener_mock)
    sut.delete_class_files(class_name, headers_base_directory, sources_base_directory, is_verbose)

    empty_subdirectory_path_after = Path(empty_subdirectory_path)

    assert os.listdir(not_empty_subdirectory_path)
    assert empty_subdirectory_path_after.exists() is False


def test_delete_class_files_will_remove_source_file(tmpdir):
    is_verbose = False
    class_name = 'class'

    headers_base_directory = f'{tmpdir}/headers'
    sources_base_directory = f'{tmpdir}/sources'

    source_file_name = f'{class_name}.cpp'
    source_file_path = f'{sources_base_directory}/{source_file_name}'

    os.mkdir(headers_base_directory)
    os.mkdir(sources_base_directory)
    open(source_file_path, 'x')

    command_runner = CommandRunner()
    file_opener_mock = MagicMock(FileOpener)

    sut = ClassFilesHelper(command_runner, file_opener_mock)
    sut.delete_class_files(class_name, headers_base_directory, sources_base_directory, is_verbose)

    source_file_path_after = Path(source_file_path)
    assert source_file_path_after.exists() is False


def test_delete_class_files_will_remove_source_file_and_its_subdirectory_when_subdirectory_is_empty(tmpdir):
    is_verbose = False
    class_subdirectory = 'subdirectory'
    class_name = f'{class_subdirectory}/class'

    headers_base_directory = f'{tmpdir}/headers'
    sources_base_directory = f'{tmpdir}/sources'

    source_file_name = f'{class_name}.cpp'
    source_file_path = f'{sources_base_directory}/{source_file_name}'
    source_file_subdirectory_path = f'{sources_base_directory}/{class_subdirectory}'

    os.mkdir(headers_base_directory)
    os.mkdir(sources_base_directory)

    os.mkdir(source_file_subdirectory_path)
    open(source_file_path, 'x')

    command_runner = CommandRunner()
    file_opener_mock = MagicMock(FileOpener)

    sut = ClassFilesHelper(command_runner, file_opener_mock)
    sut.delete_class_files(class_name, headers_base_directory, sources_base_directory, is_verbose)

    source_file_subdirectory_path_after = Path(source_file_subdirectory_path)
    assert source_file_subdirectory_path_after.exists() is False


def test_delete_class_files_will_remove_source_file_and_its_subdirectories_when_subdirectories_are_empty(tmpdir):
    is_verbose = False
    class_subdirectories = 'subdirectory1/subdirectory2'
    class_name = f'{class_subdirectories}/class'

    headers_base_directory = f'{tmpdir}/headers'
    sources_base_directory = f'{tmpdir}/sources'

    source_file_name = f'{class_name}.cpp'
    source_file_path = f'{sources_base_directory}/{source_file_name}'
    source_file_subdirectories_path = f'{sources_base_directory}/{class_subdirectories}'

    os.mkdir(headers_base_directory)
    os.mkdir(sources_base_directory)

    os.makedirs(source_file_subdirectories_path)
    open(source_file_path, 'x')

    command_runner = CommandRunner()
    file_opener_mock = MagicMock(FileOpener)

    sut = ClassFilesHelper(command_runner, file_opener_mock)
    sut.delete_class_files(class_name, headers_base_directory, sources_base_directory, is_verbose)

    assert not os.listdir(sources_base_directory)


def test_delete_class_files_will_remove_source_file_and_its_subdirectories_but_only_empty_ones(tmpdir):
    is_verbose = False
    not_empty_subdirectory = 'not_empty_subdirectory'
    additional_file = f'{not_empty_subdirectory}/additional_file.cpp'

    empty_subdirectory = 'empty_subdirectory'
    class_subdirectories = f'{not_empty_subdirectory}/{empty_subdirectory}'
    class_name = f'{class_subdirectories}/class'

    headers_base_directory = f'{tmpdir}/headers'
    sources_base_directory = f'{tmpdir}/sources'

    source_file_name = f'{class_name}.cpp'
    source_file_path = f'{sources_base_directory}/{source_file_name}'
    source_file_subdirectories_path = f'{sources_base_directory}/{class_subdirectories}'

    not_empty_subdirectory_path = f'{sources_base_directory}/{not_empty_subdirectory}'
    empty_subdirectory_path = f'{sources_base_directory}/{empty_subdirectory}'

    os.mkdir(headers_base_directory)
    os.mkdir(sources_base_directory)

    os.makedirs(source_file_subdirectories_path)
    open(f'{sources_base_directory}/{additional_file}', 'x')
    open(source_file_path, 'x')

    command_runner = CommandRunner()
    file_opener_mock = MagicMock(FileOpener)

    sut = ClassFilesHelper(command_runner, file_opener_mock)
    sut.delete_class_files(class_name, headers_base_directory, sources_base_directory, is_verbose)

    empty_subdirectory_path_after = Path(empty_subdirectory_path)

    assert os.listdir(not_empty_subdirectory_path)
    assert empty_subdirectory_path_after.exists() is False
