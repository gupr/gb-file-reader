cmake_minimum_required(VERSION 3.16)

project(gb-file-reader)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(gb-file-reader
    src/main.cpp
    src/rom_reader.cpp
    src/rom_header.cpp
    src/tile_decoder.cpp
    src/image_writer.cpp
    src/rom_corruptor.cpp
)

target_include_directories(gb-file-reader PRIVATE include)