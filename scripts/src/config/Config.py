import json
from os import path
from .ConfigNames import ConfigNames


class Config:
    config: dict
    config_path = path.join(path.dirname(__file__), 'config.json')
    temp_path = path.join(path.dirname(__file__), 'temp.json')

    def __init__(self):
        config_file = open(self.config_path, 'r')
        self.config = json.load(config_file)

    def get_default_configuration(self):
        return self.config['default_build_type']
