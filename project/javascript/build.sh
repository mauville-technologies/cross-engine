#!/bin/bash

. ../shared_scripts.sh

verify_build_folder_exists

# source emsdk
source ../../external/emsdk/emsdk_env.sh

pushd build > /dev/null || return
  emcmake cmake .. .
  make
popd > /dev/null || return