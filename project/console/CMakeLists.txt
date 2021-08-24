cmake_minimum_required(VERSION 3.6)

project(desktop)

# armeabi-v7a requires cpufeatures library
# include(AndroidNdkModules)
# android_ndk_import_module_cpufeatures()

find_package(SDL2 REQUIRED)
file(GLOB_RECURSE GLAD_SOURCES ../../external/glad/*.c)

file(GLOB_RECURSE MAIN_SOURCES ../main/*.cpp)
file(GLOB_RECURSE MAIN_HEADERS ../main/*.h)

add_executable(${PROJECT_NAME} ${MAIN_SOURCES} ${MAIN_HEADERS} ${GLAD_SOURCES})

target_include_directories(${PROJECT_NAME}
        PUBLIC
        SDL2
        SDL2main
        SDL2-static
        ../../external/glad/include
)

target_link_libraries(${PROJECT_NAME}
        PUBLIC
        SDL2
        SDL2main
        SDL2-static
        )
