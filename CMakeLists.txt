cmake_minimum_required(VERSION 3.16)

project(gb-file-reader)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(gb-file-reader
    src/main.cpp
    src/RomReader.cpp
    src/RomHeader.cpp
    src/TileDecoder.cpp
    src/ImageWriter.cpp
    src/RomCorruptor.cpp
)

target_include_directories(gb-file-reader PRIVATE include)