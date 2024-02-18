cmake_minimum_required(VERSION 3.22)

project(noodlecore
    DESCRIPTION     "Library containing core noodle functionality.")

# This section creates a static library to be compiled with external code into a larger binary.

add_library(noodlecore STATIC
    source/basicManipulations.cpp
    source/helpers.cpp
    source/layer.cpp
    source/manipulations.cpp
    source/multiplexGraph.cpp
    source/navigation.cpp
    source/selection.cpp
    source/utilities.cpp)

target_include_directories(noodlecore PRIVATE
    include/
    headers/)
