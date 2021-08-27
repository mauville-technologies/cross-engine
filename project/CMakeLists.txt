cmake_minimum_required(VERSION 3.4)

add_subdirectory(console)
add_subdirectory(android/app/jni)
# I add it here for posterity, but we should be using the build script
# add_subdirectory(javascript)