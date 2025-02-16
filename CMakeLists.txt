# Cmake Version
cmake_minimum_required(VERSION 3.10..3.31)

# Allow linking libraries to targets defined in subdirectories
cmake_policy(SET CMP0079 NEW)

# C++ Version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Define project name and version
project(fb_post VERSION 1.0
                DESCRIPTION "This is a very nice project"
                LANGUAGES CXX)

# Set the output directory for executables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

add_subdirectory(src)

