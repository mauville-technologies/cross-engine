cmake_minimum_required(VERSION 3.4)
project (cross-engine)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/dist/${CMAKE_SYSTEM_NAME})
set(CMAKE_CXX_STANDARD 20)

# Some build settings required for Javascript build
if( ${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    set(USE_FLAGS "-s USE_SDL=2 -s USE_FREETYPE=1 -s MAX_WEBGL_VERSION=2 -s MIN_WEBGL_VERSION=2")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${USE_FLAGS}")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${USE_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${USE_FLAGS}")
    set(CMAKE_EXECUTABLE_SUFFIX .html)
endif()

add_subdirectory(external/SDL)

file(GLOB_RECURSE GLAD_SOURCES external/glad/*.c)

file(GLOB_RECURSE MAIN_SOURCES main/*.cpp)
file(GLOB_RECURSE MAIN_HEADERS main/*.h)

add_executable(${PROJECT_NAME} ${MAIN_SOURCES} ${MAIN_HEADERS} ${GLAD_SOURCES})

target_include_directories(${PROJECT_NAME}
        PUBLIC
        SDL2main
        SDL2-static
        external/glad/include
        )

target_link_libraries(${PROJECT_NAME}
        PUBLIC
        SDL2main
        SDL2-static
        )

# Use the build script for android cmake file
# add_subdirectory(android/app/jni)