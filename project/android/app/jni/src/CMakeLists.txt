cmake_minimum_required(VERSION 3.6)

project(MY_APP)
set(CMAKE_CXX_STANDARD 20)

find_library(SDL2 SDL2)

add_library(main SHARED)

target_sources(main PRIVATE YourSourceHere.c)

target_link_libraries(main SDL2)


