import argparse
import sys

from ..beast_command_helper import BeastCommandHelper
from ...commandrunners.cmake import CMake


class Configure:
    def __init__(self, cwd):
        parser = argparse.ArgumentParser()
        parser.parse_args(sys.argv[2:])

        CMake.init(BeastCommandHelper.DIRECTORY_BUILD, cwd)
