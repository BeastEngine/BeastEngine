from unittest.mock import MagicMock

from src.config.config_manager import ConfigManager, Config
from src.config.config_names import ConfigNames


def test_available_names_will_return_valid_config_names():
    expected_configurations =\
        f'[{ConfigNames.CONFIG_DEBUG}, ' \
        f'{ConfigNames.CONFIG_RELEASE}, {ConfigNames.CONFIG_REL_WITH_DEBUG}, {ConfigNames.CONFIG_MIN_SIZE_REL}]'

    actual_configurations = ConfigNames.available_names()
    assert expected_configurations == actual_configurations


def test_all_configs_will_return_array_of_configurations():
    expected_configurations = [
        ConfigNames(ConfigNames.CONFIG_DEBUG),
        ConfigNames(ConfigNames.CONFIG_RELEASE),
        ConfigNames(ConfigNames.CONFIG_REL_WITH_DEBUG),
        ConfigNames(ConfigNames.CONFIG_MIN_SIZE_REL),
    ]
    actual_configurations = ConfigNames.all_configs()

    assert expected_configurations == actual_configurations
