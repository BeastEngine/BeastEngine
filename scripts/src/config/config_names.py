from enum import Enum


class ConfigNames(Enum):
    CONFIG_DEBUG = 'Debug'
    CONFIG_RELEASE = 'Release'
    CONFIG_REL_WITH_DEBUG = 'RelWithDebInfo'
    CONFIG_MIN_SIZE_REL = 'MinSizeRel'

    @staticmethod
    def from_string(name: str):
        return ConfigNames(name)

    @staticmethod
    def available_names():
        names = '['
        for name, member in ConfigNames.__members__.items():
            names += member.value
            names += ', '

        names = names[:-2]
        names += ']'
        return names

    def __str__(self):
        return self.value
