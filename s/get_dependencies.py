#!/bin/python3

from common import run_command, get_platform, pushd
import os
import certifi


def update_submodules():
    command = ["git", "submodule", "update", "--init", "--recursive"]
    run_command(command)


def install_emsdk():
    path = os.path.join('project', 'external', 'emsdk')

    path += os.path.sep
    os.environ['SSL_CERT_FILE'] = certifi.where()

    run_command([path + 'emsdk', 'install', 'latest'])
    run_command([path + 'emsdk', 'activate', 'latest'])

if __name__ == "__main__":
    # Update the submodules
    update_submodules()
    install_emsdk()
