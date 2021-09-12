# Cross Platform Engine Playground

Scratch project for figuring out how to configure and create a 3D engine
that deploys to Web, Windows and Android. Mac and iOS may come later if I some day get
a macbook.

## Requirements

- CMake
- Python3 (setup scripts)

## Setup

I'm trying to keep a set of scripts in the `\s` folder that will maintain dependencies, perform
system setup and generally allow this repository to be used straight from clone with little hassle

### Dependencies

For the most part, all the dependencies are being held as submodules. Run
`s\get_dependencies.py` to update them.

#### Android
For the android build, it is currently required to be using NDK `20.0.5594570`. I'm currently looking 
at how to get around this -- but this is it for now. (`ANDROID_NDK_HOME` && `ANDROID_HOME` need to be set too)

## Build

### Web
`s\build_web.py` -- will output a build in the `build/` folder. For some reason `emcmake`
is failing to generate the project file completely on first run. Run this script twice if
it doesn't work the first time.