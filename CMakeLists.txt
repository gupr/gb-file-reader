cmake_minimum_required(VERSION 3.16)

project(gb-rom-reader)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(gb-rom-reader
    src/main.cpp
    src/RomReader.cpp
    src/RomHeader.cpp
)