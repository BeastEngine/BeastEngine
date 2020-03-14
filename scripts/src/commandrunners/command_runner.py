import subprocess
import colorama
import shlex


class CommandRunner:
    @staticmethod
    def run_command(command, working_dir=None):
        print('\n')
        print(colorama.Fore.YELLOW + 'Running: ' + command)

        process = CommandRunner.spawn_process(shlex.split(command), working_dir)
        returned_code = CommandRunner.wait_for_process_and_print_outputs(process)

        return returned_code

    @staticmethod
    def spawn_process(args, cwd):
        return subprocess.Popen(args=args, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True, cwd=cwd)

    @staticmethod
    def wait_for_process_and_print_outputs(process):
        while True:
            stdout = process.stdout.readline()
            stderr = process.stderr.readline()
            if (stdout == '' and stderr == '') and process.poll() is not None:
                break
            if stdout:
                print(stdout.strip())
            elif stderr:
                print(colorama.Fore.RED + stderr.strip())

        return process.poll()

    # @staticmethod
    # def execute_command(command):
    #     process_result = subprocess.run(args=command, capture_output=True, text=True)
    #     output = process_result.stdout
    #     if not output:
    #         output = process_result.stderr
    #
    #     return output
