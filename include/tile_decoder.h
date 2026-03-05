#ifndef TILEDECODER_H
#define TILEDECODER_H
#include "Types.h"

class TileDecoder
{
public:
    static PixelBuffer decode_all_tiles(
        const ByteBuffer &rom,
        int tilesPerRow,
        int &outWidth,
        int &outHeight);
};

#endif