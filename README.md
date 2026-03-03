# gb-file-reader

A small C++ tool for reading, visualizing, and corrupting original Nintendo Game Boy ROM files (`.gb`).

It parses the headers from the ROM data, extracts the metadata about the game and exports raw memory as PNGs.

## Features

* Header parsing (title, cartridge type, ROM/RAM size)
* Raw or scaled PNG memory visualization
* PNG output via `stb_image_write
* Random and tunable corruption of the game ROM

## Purpose

Built for:
* Understanding how the Game Boy works
* Emulator research
* Low-level C++ practice

