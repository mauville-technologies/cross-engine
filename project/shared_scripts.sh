#!/bin/bash

verify_external_folder_exists() {
    # Navigate into the 'root' folder from our current location.
    pushd ../ > /dev/null || return
        # Check if there is no third-party folder ...
        if [ ! -d "external" ]; then
            # ... and if there isn't, create it.
            mkdir external
        fi
    popd > /dev/null || return
}

verify_build_folder_exists() {
    # Navigate into the 'root' folder from our current location.
    if [ ! -d "build" ]; then
        # ... and if there isn't, create it.
        mkdir external
    fi
}

# If required, download the SDL library source into the third-party folder.
fetch_external_lib_sdl() {
    verify_external_folder_exists

    # Navigate into the third-party folder two levels below us.
    pushd ../external > /dev/null || return
        # Check to see if there is not yet an SDL folder.
        if [ ! -d "SDL" ]; then
            SDL_VER=2.0.16
            echo "Fetching SDL (SDL2: ${SDL_VER}) ..."

            # Download the SDL2 source zip file
            wget https://www.libsdl.org/release/SDL2-${SDL_VER}.zip

            # Unzip the file into the current folder
            unzip -q SDL2-${SDL_VER}.zip

            # Rename the SDL2-${SDL_VER} folder to SDL
            mv SDL2-${SDL_VER} SDL

            # Clean up by deleting the zip file that we downloaded.
            rm SDL2-${SDL_VER}.zip
        else
            echo "SDL library already exists in third party folder."
        fi
    popd > /dev/null || return
}

fetch_external_lib_emscripten() {
    verify_external_folder_exists

    # Navigate into the third-party folder two levels below us.
    pushd ../external > /dev/null || return
        # Check to see if there is not yet an SDL folder.
        if [ ! -d "emsdk" ]; then
            echo "Fetching Emscripten ..."

            git clone https://github.com/emscripten-core/emsdk.git

            pushd emsdk > /dev/null || return
              ./emsdk install latest
              ./emsdk activate latest
              source ./emsdk_env.sh
            popd > /dev/null || return

        else
            echo "SDL library already exists in third party folder."
        fi
    popd > /dev/null || return
}