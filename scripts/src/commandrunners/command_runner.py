import subprocess
import colorama
import shlex


class CommandRunner:
    def run_command(self, command, working_dir=None):
        print('\n')
        print(colorama.Fore.YELLOW + 'Running: ' + command)

        process_result = subprocess.run(args=command, capture_output=True, text=True, cwd=working_dir)

        print(colorama.Fore.LIGHTWHITE_EX + process_result.stdout)
        print(colorama.Fore.LIGHTRED_EX + process_result.stderr)

        return process_result.returncode

    def get_command_output(self, command, working_dir=None):
        process_result = subprocess.run(args=command, capture_output=True, text=True, cwd=working_dir)

        return process_result.stdout
