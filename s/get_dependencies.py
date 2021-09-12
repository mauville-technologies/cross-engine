from common import run_command, get_platform, pushd
import os
import certifi


def update_submodules():
    command = ["git", "submodule", "update", "--init", "--recursive"]
    run_command(command)


def install_emsdk():
    path = os.path.join('project', 'external', 'emsdk')

    with pushd(path):
        os.environ['SSL_CERT_FILE'] = certifi.where()
        extension = ''

        if get_platform() == "windows":
            extension = '.bat'

        run_command(['emsdk', 'install', 'latest'])

if __name__ == "__main__":
    # Update the submodules
    update_submodules()
    install_emsdk()
