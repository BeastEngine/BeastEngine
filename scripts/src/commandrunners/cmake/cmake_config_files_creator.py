from src.config.ConfigManager import Config, ConfigFiles
from src.files.file_opener import FileOpener
from src.commandrunners.command_runner import CommandRunner


class CMakeConfigFilesCreator:
    def __init__(self, command_runner: CommandRunner, file_opener: FileOpener):
        self.command_runner = command_runner
        self.file_opener = file_opener

    def generate_main_config(self, cmake_config: Config.CMake, cmake_config_dir: str):
        config_files = self.__get_config_files_full_paths(cmake_config_dir, cmake_config.config_files)
        self.__copy_config(config_files)

        config_file = self.file_opener.open(config_files.filename)
        config_file.replace_content(self.__prepare_main_config_content(cmake_config, config_file))

    def generate_target_config(self, target_config: Config.CMake.Target, cmake_config_dir: str):
        if target_config.config_files is None or target_config.directories is None:
            return

        config_files = self.__get_config_files_full_paths(cmake_config_dir, target_config.config_files)
        self.__copy_config(config_files)

        # Create mappings: cmake_config_variable_content_placeholder -> actual_value_that_should_be_set
        directories = target_config.directories
        headers = target_config.headers
        source = target_config.sources
        vars_names_map = {
            directories.include_directory_placeholder: directories.include_directory,
            directories.source_directory_placeholder: directories.source_directory,
            headers.files_list_placeholder: self.__get_target_files(headers),
            source.files_list_placeholder: self.__get_target_files(source),
        }

        config_file = self.file_opener.open(config_files.filename)
        config_file.replace_content(config_file.get_content().format_map(vars_names_map))

    def __get_config_files_full_paths(self, config_dir: str, config_files: ConfigFiles):
        full_path_files = ConfigFiles()
        full_path_files.dist_filename = config_dir + '/' + config_files.dist_filename
        full_path_files.filename = config_dir + '/' + config_files.filename

        return full_path_files

    def __copy_config(self, config_files: ConfigFiles):
        copy_command = self.__get_copy_command(config_files.dist_filename, config_files.filename)
        self.command_runner.run_command(copy_command)

    def __prepare_main_config_content(self, cmake_config: Config.CMake, config_file: FileOpener.File):
        project_config = cmake_config.project
        target_names_map = {
            project_config.project_name_placeholder: project_config.name,
            project_config.version_major_placeholder: project_config.version_major,
            project_config.version_minor_placeholder: project_config.version_minor,
            project_config.version_patch_placeholder: project_config.version_patch,
            cmake_config.lib.target_name_placeholder: cmake_config.lib.target_name,
            cmake_config.exe.target_name_placeholder: cmake_config.exe.target_name,
        }

        return config_file.get_content().format_map(target_names_map)

    @staticmethod
    def __get_copy_command(source, destination):
        return f"cp {source} {destination}"

    @staticmethod
    def __get_target_files(target_files: Config.CMake.Target.Files):
        # Four space indentation and one new line character
        number_of_characters_to_remove = 5
        four_space_indent = '    '

        files_var = ""
        for file in target_files.files:
            if target_files.base_dir.__len__() == 0:
                files_var += '"' + file
            else:
                files_var += f'"{target_files.base_dir}/' + file
            files_var += '"\n' + four_space_indent

        return files_var[:-number_of_characters_to_remove]
