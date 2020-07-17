import os

from src.files.file_opener import FileOpener
from src.commandrunners.command_runner import CommandRunner
from src.config.config_manager import Config
from src.functions import get_project_path
from pathlib import Path


class ClassFilesHelper:
    HEADER_FILE_EXTENSION = 'h'
    SOURCE_FILE_EXTENSION = 'cpp'
    CLASS_NAME_DIRECTORY_SEPARATOR = '/'

    COMMAND_CREATE_FILE = 'touch'
    COMMAND_CREATE_DIRECTORY = 'mkdir'
    COMMAND_REMOVE_FILE = 'rm'
    COMMAND_REMOVE_DIRECTORY = 'rm -r'

    def __init__(self, command_runner: CommandRunner, file_opener: FileOpener):
        self.command_runner = command_runner
        self.file_opener = file_opener

    def class_files_exist(self, target_config: Config.CMake.Target, class_name: str):
        headers_files = target_config.headers.files
        sources_files = target_config.sources.files

        header_file = self.get_header_file_name(class_name)
        source_file = self.get_source_file_name(class_name)

        return headers_files.__contains__(header_file) and sources_files.__contains__(source_file)

    def create_class_files(
            self,
            class_name: str,
            headers_base_dir: str,
            sources_base_dir: str,
            is_verbose: bool,
            namespace=None
    ):
        cwd = get_project_path()

        if class_name.find(self.CLASS_NAME_DIRECTORY_SEPARATOR) != -1:
            self.__create_class_sub_directories(class_name, headers_base_dir, sources_base_dir, is_verbose)

        header_file_name = self.get_header_file_name(class_name)
        header_file_path = f'{headers_base_dir}/{header_file_name}'

        source_file_name = self.get_source_file_name(class_name)
        source_file_path = f'{sources_base_dir}/{source_file_name}'

        self.__create_header_file(header_file_path, cwd, is_verbose, namespace)
        self.__create_source_file(source_file_path, header_file_name, cwd, is_verbose, namespace)

    def delete_class_files(self, class_name: str, headers_base_dir: str, sources_base_dir: str, is_verbose: bool):
        header_file_name = self.get_header_file_name(class_name)
        self.command_runner.run_command(f'{self.COMMAND_REMOVE_FILE} {header_file_name}', headers_base_dir, is_verbose)

        source_file_name = self.get_source_file_name(class_name)
        self.command_runner.run_command(f'{self.COMMAND_REMOVE_FILE} {source_file_name}', sources_base_dir, is_verbose)

        if class_name.find(self.CLASS_NAME_DIRECTORY_SEPARATOR) == -1:
            return

        class_subdirectories_path = self.__get_class_subdirectories_path(class_name)

        self.__delete_class_subdirectories(class_subdirectories_path, headers_base_dir, is_verbose)
        self.__delete_class_subdirectories(class_subdirectories_path, sources_base_dir, is_verbose)

    def get_header_file_name(self, class_name):
        return f'{class_name}.{self.HEADER_FILE_EXTENSION}'

    def get_source_file_name(self, class_name):
        return f'{class_name}.{self.SOURCE_FILE_EXTENSION}'

    def __create_class_sub_directories(self, class_name: str, headers_base_dir: str, sources_base_dir: str, is_verbose: bool):
        class_sub_directories_path = self.__get_class_subdirectories_path(class_name)

        headers_path = Path(f'{headers_base_dir}/{class_sub_directories_path}')
        sources_path = Path(f'{sources_base_dir}/{class_sub_directories_path}')

        if not headers_path.exists():
            self.command_runner \
                .run_command(f'{self.COMMAND_CREATE_DIRECTORY} {class_sub_directories_path}', headers_base_dir, is_verbose)

        if not sources_path.exists():
            self.command_runner \
                .run_command(f'{self.COMMAND_CREATE_DIRECTORY} {class_sub_directories_path}', sources_base_dir, is_verbose)

    def __delete_class_subdirectories(self, path: str, cwd: str, is_verbose: bool):
        full_path = f'{cwd}/{path}'
        if os.path.exists(full_path) and os.path.isdir(full_path) and not os.listdir(full_path):
            self.command_runner.run_command(f'{self.COMMAND_REMOVE_DIRECTORY} {path}', cwd, is_verbose)

        if path.find(self.CLASS_NAME_DIRECTORY_SEPARATOR) != -1:
            last_occurrence = path.rfind(self.CLASS_NAME_DIRECTORY_SEPARATOR)
            self.__delete_class_subdirectories(path[:last_occurrence], cwd, is_verbose)

    def __create_header_file(self, header_file_path: str, cwd: str, is_verbose: bool, namespace):
        file_content = '#pragma once'
        if namespace is not None:
            file_content += f'\n\nnamespace {namespace}\n{{\n\n}}\n'

        self.command_runner.run_command(f'{self.COMMAND_CREATE_FILE} {header_file_path}', cwd, is_verbose)
        header_file = self.file_opener.open(header_file_path)
        header_file.replace_content(file_content)

    def __create_source_file(self, source_file_path: str, header_file_name: str, cwd: str, is_verbose: bool, namespace):
        file_content = f'#include "BeastEngine/{header_file_name}"'
        if namespace is not None:
            file_content += f'\n\nnamespace {namespace}\n{{\n\n}}\n'

        self.command_runner.run_command(f'{self.COMMAND_CREATE_FILE} {source_file_path}', cwd, is_verbose)
        source_file = self.file_opener.open(source_file_path)
        source_file.replace_content(file_content)

    def __get_class_subdirectories_path(self, class_name):
        directories = class_name.split(self.CLASS_NAME_DIRECTORY_SEPARATOR)
        directories.pop()

        class_sub_directories_path = ''
        for directory in directories:
            class_sub_directories_path += f'{directory}/'

        return class_sub_directories_path[:-1]
