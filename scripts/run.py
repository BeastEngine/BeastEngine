import colorama

from src.beastengine.commands.class_commands.class_files_helper import ClassFilesHelper
from src.config.target_config_manager import TargetConfigManager
from src.functions import get_project_path, get_config_path, get_build_dir_path
from src.commandrunners.cmake.cmake import CMake
from src.commandrunners.cmake.cmake_config_files_creator import CMakeConfigFilesCreator
from src.commandrunners.conan import Conan
from src.config.config_manager import ConfigManager
from src.commandrunners.command_runner import CommandRunner
from src.files.file_opener import FileOpener
from src.json_utils.json_manager import JSONManager
from src.beastengine.beastengine import BeastEngine
from src.beastengine.commands.class_commands.target_cmake_vars_file_opener import TargetCMakeVarsFileOpener

colorama.init(autoreset=True)

project_working_dir = get_project_path()
build_dir = get_build_dir_path()

file_opener = FileOpener()
config_manager = ConfigManager(get_config_path(), JSONManager(file_opener))
command_runner = CommandRunner()

conan = Conan(command_runner, build_dir)
cmake =\
    CMake(
        command_runner,
        CMakeConfigFilesCreator(command_runner, file_opener),
        config_manager,
        project_working_dir,
        build_dir
    )

target_config_manager = TargetConfigManager(TargetCMakeVarsFileOpener(file_opener))
class_files_helper = ClassFilesHelper(command_runner, file_opener)

BeastEngine(
    project_working_dir,
    build_dir,
    command_runner,
    config_manager,
    conan,
    cmake,
    target_config_manager,
    class_files_helper
)

colorama.deinit()