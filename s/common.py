import contextlib
import subprocess
import platform

import contextlib
import os


@contextlib.contextmanager
def pushd(new_dir):
    old_dir = os.getcwd()
    os.chdir(new_dir)
    try:
        yield
    finally:
        os.chdir(old_dir)


def run_command(command, env=os.environ):

    if get_platform().lower() == "linux":
        new_command = "/bin/bash -c '" + " ".join(command) + "'"
        os.system(new_command)
    else:
        process = subprocess.Popen(command, stdout=subprocess.PIPE, env=env, shell=True)

        for line in process.stdout:
            print(line.decode('utf-8'), end='')


def build_command_list(*commands):
    new_command = []

    for i in range(len(commands)):
        if i != 0:
            new_command += ['&&'] + commands[i]
        else:
            new_command += commands[i]

    return new_command

def get_platform():
    return platform.system().lower()
