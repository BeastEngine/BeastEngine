import colorama


class BeastCommandHelper:
    DIRECTORY_BUILD = "build"

    COMMAND_NAME_INIT = 'init'
    COMMAND_NAME_CONFIGURE = 'configure'
    COMMAND_NAME_BUILD = 'build'
    COMMAND_NAME_CLASS = 'class'
    COMMAND_NAME_CLASS_ADD = 'add'
    COMMAND_NAME_CLASS_REMOVE = 'remove'

    @staticmethod
    def format_text(text: str, substitution_map=None):
        mappings = {
            'red': colorama.Fore.LIGHTRED_EX,
            'green': colorama.Fore.LIGHTGREEN_EX,
            'white': colorama.Fore.LIGHTWHITE_EX,
            'purple': colorama.Fore.LIGHTMAGENTA_EX,
            'yellow': colorama.Fore.YELLOW,
            'reset': colorama.Fore.RESET,
            'init': BeastCommandHelper.COMMAND_NAME_INIT,
            'configure': BeastCommandHelper.COMMAND_NAME_CONFIGURE,
            'build': BeastCommandHelper.COMMAND_NAME_BUILD,
            'class': BeastCommandHelper.COMMAND_NAME_CLASS,
            'class_add': BeastCommandHelper.COMMAND_NAME_CLASS_ADD,
            'class_remove': BeastCommandHelper.COMMAND_NAME_CLASS_REMOVE,
        }

        if substitution_map is not None:
            mappings.update(substitution_map)

        return text.format_map(mappings)
