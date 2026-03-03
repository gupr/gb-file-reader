#pragma once
#include <vector>
#include <cstdint>

struct Pixel
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

using ByteBuffer = std::vector<uint8_t>;
using PixelBuffer = std::vector<Pixel>;