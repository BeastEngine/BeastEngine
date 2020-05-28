from argparse import ArgumentParser
from inspect import currentframe, getframeinfo
from pathlib import Path


def get_project_path():
    current_filename = getframeinfo(currentframe()).filename
    return Path(current_filename).resolve().parent.parent.parent.__str__()


def get_build_dir_name():
    return "build"


def get_build_dir_path():
    return f"{get_project_path()}/{get_build_dir_name()}"


def get_config_path():
    return f"{get_project_path()}/config/config.json"


def is_verbose_set(args):
    return args.verbose


def create_arguments_parser(program=None, usage=None, description=None, formatter_class=None):
    if formatter_class is None:
        parser = ArgumentParser(prog=program, usage=usage, description=description)
    else:
        parser = ArgumentParser(prog=program, usage=usage, description=description, formatter_class=formatter_class)

    parser.add_argument('-v', '--verbose', help="show command output", action="store_true")
    return parser
