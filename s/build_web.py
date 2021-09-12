import pathlib
import os
from common import build_command_list, pushd, run_command, get_platform

if __name__ == "__main__":

    # make sure build folder exists
    build_path = os.path.join(os.getcwd(), "build")
    pathlib.Path('build').mkdir(exist_ok=True, parents=True)

    path_to_emsdk = os.path.join(os.getcwd(), 'project', 'external', 'emsdk', 'emsdk')

    with pushd(build_path):
        run_command(build_command_list([path_to_emsdk, 'activate', 'latest'],
                                       ['emcmake', 'cmake', '..', '.']))

        platform = get_platform()
        if platform == "windows":
            run_command(['ninja'])
        else:
            run_command(['make'])

