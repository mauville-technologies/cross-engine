#!/bin/python3

import pathlib
import os
from common import build_command_list, pushd, run_command, get_platform

if __name__ == "__main__":

    # make sure build folder exists
    build_path = os.path.join(os.getcwd(), "build")
    pathlib.Path('build').mkdir(exist_ok=True, parents=True)

    path_to_emsdk = os.path.join(os.getcwd(), 'project', 'external', 'emsdk')

    platform = get_platform()

    if platform == "windows":
        run_command(build_command_list([os.path.join(path_to_emsdk,'emsdk'), 'activate', 'latest'], ['emcmake', 'cmake', '..', '.']))
        run_command(['ninja'])
    elif platform == "linux":
        run_command(build_command_list(['pushd', path_to_emsdk], ['.', os.path.join(path_to_emsdk,'emsdk_env.sh')], ['popd'],
                                       ['pushd', build_path], ['emcmake', 'cmake', '..', '.'], ['popd']))
        run_command(build_command_list(['pushd', build_path], ['make'], ['popd']))

