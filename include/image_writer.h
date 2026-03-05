#pragma once
#include <string>
#include "types.h"

class ImageWriter
{
public:
    static void write_png(
        const std::string &path,
        const PixelBuffer &pixels,
        int width,
        int height,
        const std::string &title,
        size_t romSizeBytes);

private:
    static void render_footer(
        std::vector<uint8_t> &rgb,
        int width,
        int height,
        int footerHeight,
        const std::string &text);
};