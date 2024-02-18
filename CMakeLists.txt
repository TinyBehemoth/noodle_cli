cmake_minimum_required (VERSION 3.22)

project (noodle)

# The noodlecore library
# ----------------------
add_subdirectory("noodlecore/")

# Command Line Interface
# ----------------------
file(GLOB CliSourceFiles
    "source/main.cpp"

    "source/interactive.cpp"
    "source/cliSession.cpp"
    "source/cliDisplay.cpp"
    "source/helpers.cpp"

    "source/exporter.cpp"
    "source/importer.cpp"

    "source/core wrappers/basic.cpp"
    "source/core wrappers/layer.cpp"
    "source/core wrappers/manipulations.cpp"
    "source/core wrappers/navigation.cpp"
    "source/core wrappers/selection.cpp"
    "source/core wrappers/utilities.cpp"
)
add_executable(${PROJECT_NAME} ${CliSourceFiles})
target_include_directories(${PROJECT_NAME} PRIVATE
    "headers/"
    "headers/core wrappers/"
    "noodlecore/include/"
)
# Requires libreadline-dev to be installed on the system.
# Otherwise modify this CMakeLists.txt file to include it by some other method.
target_link_libraries(${PROJECT_NAME}
    noodlecore
    readline
)
