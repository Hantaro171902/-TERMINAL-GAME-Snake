cmake_minimum_required(VERSION 3.10)

# Project name and version
project(TerminalSnake VERSION 1.0)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set source and header files
set(SOURCES
    src/main.cpp
    src/ultils.cpp
    src/snake.cpp
    src/color.cpp
    src/brick.cpp
)

set(HEADERS
    src/headers/snake.hpp
    src/headers/ultils.hpp
    src/headers/color.hpp
    src/headers/brick.hpp
)

# Exe
add_executable(Snake ${SOURCES} ${HEADERS})

target_include_directories(Snake PRIVATE src/headers)

# Optional: copy asset folder into build dir
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
