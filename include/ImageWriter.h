#pragma once
#include <string>
#include "types.h"

class ImageWriter
{
public:
    static void WritePNG(
        const std::string &path,
        const PixelBuffer &pixels,
        int width,
        int height,
        const std::string &title,
        size_t romSizeBytes);

private:
    static void RenderFooter(
        std::vector<uint8_t> &rgb,
        int width,
        int height,
        int footerHeight,
        const std::string &text);
};