import colorama
from inspect import currentframe, getframeinfo
from pathlib import Path

from src.beastengine.beastengine import BeastEngine

colorama.init(autoreset=True)

current_filename = getframeinfo(currentframe()).filename
project_working_dir = Path(current_filename).resolve().parent.parent.__str__()

BeastEngine(project_working_dir)

colorama.deinit()
