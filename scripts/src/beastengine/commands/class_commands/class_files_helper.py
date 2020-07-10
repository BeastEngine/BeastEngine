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

    def get_header_file_name(self, class_name):
        return f'{class_name}.{self.HEADER_FILE_EXTENSION}'

    def get_source_file_name(self, class_name):
        return f'{class_name}.{self.SOURCE_FILE_EXTENSION}'

    def __create_class_sub_directories(self, class_name: str, headers_base_dir: str, sources_base_dir: str, is_verbose: bool):
        directories = class_name.split(self.CLASS_NAME_DIRECTORY_SEPARATOR)
        directories.pop()

        class_sub_directory_path = ''
        for directory in directories:
            class_sub_directory_path += f'{directory}/'

        class_sub_directory_path = class_sub_directory_path[:-1]

        headers_path = Path(f'{headers_base_dir}/{class_sub_directory_path}')
        sources_path = Path(f'{sources_base_dir}/{class_sub_directory_path}')

        if headers_path.exists() or sources_path.exists():
            return

        self.command_runner\
            .run_command(f'{self.COMMAND_CREATE_DIRECTORY} {class_sub_directory_path}', headers_base_dir, is_verbose)
        self.command_runner\
            .run_command(f'{self.COMMAND_CREATE_DIRECTORY} {class_sub_directory_path}', sources_base_dir, is_verbose)

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
