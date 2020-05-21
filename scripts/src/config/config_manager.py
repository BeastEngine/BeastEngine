from typing import Optional

from src.beastengine.json.json_manager import JSONManager


class ConfigFiles:
    dist_filename: str
    filename: str


class Config:
    class CMake:
        class Project:
            name: str
            version_major: str
            version_minor: str
            version_patch: str
            name_placeholder: str
            version_major_placeholder: str
            version_minor_placeholder: str
            version_patch_placeholder: str

        class Target:
            class Directories:
                include_directory_placeholder: str
                include_directory: str
                source_directory_placeholder: str
                source_directory: str

            class Files:
                base_dir: str
                files_list_placeholder: str
                files: list

            target_name: str
            target_name_placeholder: str
            config_files: Optional[ConfigFiles]
            directories: Optional[Directories]
            headers: Files
            sources: Files

        directory_name: str
        config_files: ConfigFiles
        project: Project
        lib: Target
        exe: Target

    default_build_type = str
    cmake: CMake


class ConfigManager:
    JSON_STR_INDENT = 2

    json_config: dict
    config: Config

    def __init__(self, config_path, json_manager: JSONManager):
        self.config_path = config_path
        self.json_manager = json_manager
        self.json_config = json_manager.load_from_file(config_path)

        self.__generate_config_objects()

    def update_config(self):
        self.json_manager.save_to_file(self.config, self.config_path, self.JSON_STR_INDENT)

    def __generate_config_objects(self):
        self.config = Config()
        self.config.default_build_type = self.json_config['default_build_type']

        # General CMake config variables
        cmake_config = self.json_config['cmake_config']
        self.config.cmake = Config.CMake()
        self.config.cmake.directory_name = cmake_config['directory_name']
        self.config.cmake.config_files = self.__create_config_files(cmake_config['config_files'])

        # CMake project config
        self.config.cmake.project = self.__create_cmake_project_config(cmake_config['project'])

        # Targets config variables
        targets_config = cmake_config['targets']
        self.config.cmake.lib = self.__create_target_config(targets_config['lib'])
        self.config.cmake.exe = self.__create_target_config(targets_config['exe'])

    @staticmethod
    def __create_cmake_project_config(project_config):
        project = Config.CMake.Project()

        project.name = project_config['name']
        project.version_major = project_config['version_major']
        project.version_minor = project_config['version_minor']
        project.version_patch = project_config['version_patch']

        project.name_placeholder = project_config['name_placeholder']
        project.version_major_placeholder = project_config['version_major_placeholder']
        project.version_minor_placeholder = project_config['version_minor_placeholder']
        project.version_patch_placeholder = project_config['version_patch_placeholder']

        return project

    @staticmethod
    def __create_target_config(target_config):
        target = Config.CMake.Target()
        target.target_name = target_config['target_name']
        target.target_name_placeholder = target_config['target_name_placeholder']
        target.config_files = ConfigManager.__create_config_files(target_config['config_files'])

        target.headers = ConfigManager.__create_target_files(target_config['headers'])
        target.sources = ConfigManager.__create_target_files(target_config['sources'])

        target.directories = ConfigManager.__create_target_directories(target_config['directories'])

        return target

    @staticmethod
    def __create_config_files(config_files):
        if config_files is None:
            return None

        files = ConfigFiles()
        files.dist_filename = config_files['dist_filename']
        files.filename = config_files['filename']

        return files

    @staticmethod
    def __create_target_files(files):
        target_files = Config.CMake.Target.Files()
        target_files.base_dir = files['base_dir']
        target_files.files_list_placeholder = files['files_list_placeholder']
        target_files.files = files['files']

        return target_files

    @staticmethod
    def __create_target_directories(target_directories):
        if target_directories is None:
            return None

        directories = Config.CMake.Target.Directories()
        directories.include_directory_placeholder = target_directories['include_directory_placeholder']
        directories.include_directory = target_directories['include_directory']

        directories.source_directory_placeholder = target_directories['source_directory_placeholder']
        directories.source_directory = target_directories['source_directory']

        return directories
