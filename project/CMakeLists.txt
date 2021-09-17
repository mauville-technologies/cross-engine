cmake_minimum_required(VERSION 3.10)

project (cross-engine)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/dist/${CMAKE_SYSTEM_NAME})

# Android studio is using an old version of CMake that doesn't allow c++20
# I couldnt fix it so I did this.
if ( NOT ${CMAKE_SYSTEM_NAME} MATCHES "Android")
    set(CMAKE_CXX_STANDARD 20)
endif()

# Some build settings required for Javascript build
if( ${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    set(USE_FLAGS "-s USE_SDL=2 -s USE_FREETYPE=1 -s MAX_WEBGL_VERSION=2 -s MIN_WEBGL_VERSION=2 --preload-file ${PROJECT_SOURCE_DIR}/assets@/assets")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${USE_FLAGS}")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${USE_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${USE_FLAGS}")
    set(CMAKE_EXECUTABLE_SUFFIX .html)
endif()

add_subdirectory(external/SDL)
add_subdirectory(external/glm)

file(GLOB_RECURSE GLAD_SOURCES external/glad/*.c)

file(GLOB_RECURSE MAIN_SOURCES main/*.cpp)
file(GLOB_RECURSE MAIN_HEADERS main/*.h)

if ( ${CMAKE_SYSTEM_NAME} MATCHES "Android")
    add_library(${PROJECT_NAME} SHARED ${MAIN_SOURCES} ${MAIN_HEADERS} ${GLAD_SOURCES})
else()
    add_executable(${PROJECT_NAME} ${MAIN_SOURCES} ${MAIN_HEADERS} ${GLAD_SOURCES})
endif()

target_include_directories(${PROJECT_NAME}
        PUBLIC

        external/glad/include
        glm
        PRIVATE
            SDL2main
            SDL2-static
            main
        )

target_link_libraries(${PROJECT_NAME}
        PUBLIC
        glm
        PRIVATE
        SDL2main
        SDL2-static
        )

# Asset management
# The various platforms will have different locations where they expect
# Their assets to be located.
if ( ${CMAKE_SYSTEM_NAME} MATCHES "Android")
    add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${PROJECT_SOURCE_DIR}/assets/ $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets)
else()
    add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${PROJECT_SOURCE_DIR}/assets/ $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets)
endif()