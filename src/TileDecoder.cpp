#include "TileDecoder.h"
#include <stdexcept>

PixelBuffer TileDecoder::DecodeAllTiles(
    const ByteBuffer &rom,
    int tilesPerRow,
    int &outWidth,
    int &outHeight)
{
    if (tilesPerRow <= 0)
        throw std::runtime_error("tilesPerRow must be > 0");

    const int bytesPerTile = 16;
    const int tileSize = 8;

    size_t tileCount = rom.size() / bytesPerTile;
    if (tileCount == 0)
        throw std::runtime_error("ROM too small to contain tiles.");

    int tileRows = static_cast<int>((tileCount + tilesPerRow - 1) / tilesPerRow);

    outWidth = tilesPerRow * tileSize;
    outHeight = tileRows * tileSize;

    PixelBuffer image(outWidth * outHeight);

    for (size_t tileIndex = 0; tileIndex < tileCount; ++tileIndex)
    {
        size_t base = tileIndex * bytesPerTile;

        int tileX = tileIndex % tilesPerRow;
        int tileY = tileIndex / tilesPerRow;

        for (int row = 0; row < 8; ++row)
        {
            uint8_t byte1 = rom[base + row * 2];
            uint8_t byte2 = rom[base + row * 2 + 1];

            for (int bit = 7; bit >= 0; --bit)
            {
                uint8_t bit0 = (byte1 >> bit) & 1;
                uint8_t bit1 = (byte2 >> bit) & 1;
                uint8_t colorIndex = (bit1 << 1) | bit0;

                uint8_t shade;

                // Grayscale mapping
                switch (colorIndex)
                {
                case 0:
                    shade = 255;
                    break; // white
                case 1:
                    shade = 170;
                    break;
                case 2:
                    shade = 85;
                    break;
                case 3:
                    shade = 0;
                    break; // black
                default:
                    shade = 255;
                }

                int pixelX = tileX * 8 + (7 - bit);
                int pixelY = tileY * 8 + row;

                size_t pixelIndex = pixelY * outWidth + pixelX;

                if (pixelIndex < image.size())
                {
                    image[pixelIndex] = {shade, shade, shade};
                }
            }
        }
    }

    return image;
}