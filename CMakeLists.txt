cmake_minimum_required(VERSION 3.10)

# Project Name
project(collobrative-notepad)

# Set C++ Standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message(STATUS "Configuring for Linux")
# Include directories for uSockets and uWebSockets
include_directories(/usr/local/include/uWebSockets)

set(SOURCES
src/notepadSever.cpp
)


# Add Executable
add_executable(collobrative-notepad ${SOURCES})

# Linking against uWebSockets, OpenSSL, Zlib, and pthread
target_link_libraries(collobrative-notepad PRIVATE 
        -l:uSockets.a 
        ssl 
        crypto 
        z 
        pthread
    )