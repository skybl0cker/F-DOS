cmake_minimum_required(VERSION 3.10)
project(DosEmulator)

# Set C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)

# Add all source files here
set(SOURCES
    src/main.cpp
    src/shell.cpp
    src/pong.cpp
    src/snake.cpp
)

# Create the executable
add_executable(dos_emulator ${SOURCES})

