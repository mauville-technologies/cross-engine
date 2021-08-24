cmake_minimum_required(VERSION 3.6)

project(main)

# armeabi-v7a requires cpufeatures library
# include(AndroidNdkModules)
# android_ndk_import_module_cpufeatures()


# SDL sources are in a subfolder named "SDL"
add_subdirectory(SDL)

# Compilation of companion libraries
#add_subdirectory(SDL_image)
#add_subdirectory(SDL_mixer)
#add_subdirectory(SDL_ttf)

file(GLOB_RECURSE GLAD_SOURCES ../../../../external/glad/*.c)

file(GLOB_RECURSE MAIN_SOURCES ../../../main/*.cpp)
file(GLOB_RECURSE MAIN_HEADERS ../../../main/*.h)

# Your game and its CMakeLists.txt are in a subfolder named "src"
add_library(${PROJECT_NAME} SHARED ${MAIN_SOURCES} ${MAIN_HEADERS} ${GLAD_SOURCES})

target_include_directories(${PROJECT_NAME}
        PUBLIC
            SDL2
            SDL2main
            SDL2-static
            ../../../../external/glad/include
)

target_link_libraries(${PROJECT_NAME}
        PUBLIC
            SDL2
            SDL2main
            SDL2-static
)
