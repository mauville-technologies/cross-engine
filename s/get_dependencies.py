#!/bin/python

from common import run_command, get_platform, pushd
import os

if get_platform() == "linux":
    import certifi


def update_submodules():
    command = ["git", "submodule", "update", "--init", "--recursive"]
    run_command(command)


def install_emsdk():
    path = os.path.join('project', 'external', 'emsdk')

    path += os.path.sep

    if get_platform() == "linux":
        os.environ['SSL_CERT_FILE'] = certifi.where()

    run_command([path + ('emsdk' if get_platform() != "windows" else "emsdk.bat"), 'install', 'latest'])
    run_command([path + ('emsdk' if get_platform() != "windows" else "emsdk.bat"), 'activate', 'latest'])

if __name__ == "__main__":
    # Update the submodules
    update_submodules()
    install_emsdk()
