from .command_runner import CommandRunner
from ..config.ConfigManager import ConfigManager, Config, ConfigFiles
from ..config.ConfigNames import ConfigNames


class CMake:
    COMMAND_INIT = "cmake -S . -B {}"
    COMMAND_BUILD = "cmake --build . --config {}"
    COMMAND_CREATE_CONFIG = "cp {} {}"

    TARGETS_FILES_SEPARATOR = '# Files'
    FILE_MODE_READ = 'r'
    FILE_MODE_READ_APPEND_WRITE = 'r+'
    FILE_MODE_TRUNCATE_WRITE = 'w'

    def __init__(self, command_runner: CommandRunner, config_manager: ConfigManager, project_dir, build_dir):
        self.command_runner = command_runner
        self.config = config_manager.config.cmake
        self.config_dir_name = project_dir + '/' + self.config.directory_name

        self.project_dir = project_dir
        self.build_dir = build_dir

    def configure(self):
        self.command_runner.run_command(CMake.COMMAND_INIT.format(self.build_dir), self.project_dir)

    @staticmethod
    def build(build_type: ConfigNames, cwd: str):
        CommandRunner.run_command(CMake.COMMAND_BUILD.format(build_type), cwd)

    def generate_configs(self):
        self.generate_main_config()
        self.generate_targets_configs()

    def generate_main_config(self):
        config_files = self.__get_config_files_full_paths(self.config.config_files)
        config_file_content = self.__prepare_main_config_content(config_files)

        config_file = open(config_files.filename, self.FILE_MODE_TRUNCATE_WRITE)
        config_file.write(config_file_content)
        config_file.close()

    def generate_targets_configs(self):
        if self.config.lib.config_files is not None:
            self.generate_target_config(self.config.lib)

        if self.config.exe.config_files is not None:
            self.generate_target_config(self.config.exe)

    def generate_target_config(self, target: Config.CMake.Target):
        if target.directories is None:
            return

        # Copy config.cmake.dist file into the config.cmake file
        config_files = self.__get_config_files_full_paths(target.config_files)
        config_command = self.__get_copy_command(config_files.dist_filename, config_files.filename)
        CommandRunner.run_command(config_command)

        # Create mappings: cmake_config_variable_content_placeholder -> actual_value_that_should_be_set
        directories = target.directories
        headers = target.headers
        source = target.sources
        vars_names_map = {
            directories.include_directory_placeholder: directories.include_directory,
            directories.source_directory_placeholder: directories.source_directory,
            headers.files_list_placeholder: self.__get_target_files(headers),
            source.files_list_placeholder: self.__get_target_files(source),
        }

        # Open the config file and replace the placeholders with proper values
        config_file = open(config_files.filename, self.FILE_MODE_READ_APPEND_WRITE)
        config_file_content = config_file.read().format_map(vars_names_map)
        config_file.truncate(0)
        config_file.seek(0)
        config_file.write(config_file_content)
        config_file.close()

    def __prepare_main_config_content(self, config_files: ConfigFiles):
        config_command = self.__get_copy_command(config_files.dist_filename, config_files.filename)
        CommandRunner.run_command(config_command)

        project_config = self.config.project
        target_names_map = {
            project_config.project_name_placeholder: project_config.name,
            project_config.version_major_placeholder: project_config.version_major,
            project_config.version_minor_placeholder: project_config.version_minor,
            project_config.version_patch_placeholder: project_config.version_patch,
            self.config.lib.target_name_placeholder: self.config.lib.target_name,
            self.config.exe.target_name_placeholder: self.config.exe.target_name,
        }

        dist_config_file = open(config_files.dist_filename, self.FILE_MODE_READ)
        config_file_content = dist_config_file.read().format_map(target_names_map)
        dist_config_file.close()

        return config_file_content

    def __get_config_files_full_paths(self, config_files: ConfigFiles):
        full_path_files = ConfigFiles()
        full_path_files.dist_filename = self.config_dir_name + '/' + config_files.dist_filename
        full_path_files.filename = self.config_dir_name + '/' + config_files.filename

        return full_path_files

    @staticmethod
    def __get_target_files(target_files: Config.CMake.Target.Files):
        files_var = ""
        for file in target_files.files:
            if target_files.base_dir.__len__() == 0:
                files_var += '"' + file + '"\n'
            else:
                files_var += f'"{target_files.base_dir}/' + file + '"\n'

        return files_var[:-1]

    @staticmethod
    def __create_cmake_variable(name, content):
        return f"SET({name} {content})"

    @staticmethod
    def __get_copy_command(source, destination):
        return f"cp {source} {destination}"
