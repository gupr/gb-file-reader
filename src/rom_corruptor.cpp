#include "rom_corruptor.h"
#include <random>
#include <algorithm>

static constexpr size_t SAFE_ZONE_END = 0x10000;
static constexpr size_t TILE_SIZE = 16;

void RomCorruptor::corrupt(std::vector<uint8_t> &romData,
                           double percent,
                           double intensity,
                           uint32_t seed)
{
    if (percent <= 0.0 || percent > 100.0)
        return;

    if (intensity <= 0.0)
        return;

    if (romData.size() <= SAFE_ZONE_END)
        return;

    size_t start = SAFE_ZONE_END;
    size_t totalTiles = (romData.size() - start) / TILE_SIZE;

    if (totalTiles == 0)
        return;

    size_t tilesToCorrupt =
        static_cast<size_t>((percent / 100.0) * totalTiles);

    if (tilesToCorrupt == 0)
        return;

    // RNG
    std::mt19937 rng(seed ? seed : std::random_device{}());

    // Create unique tile index list
    std::vector<size_t> indices(totalTiles);
    for (size_t i = 0; i < totalTiles; ++i)
        indices[i] = i;

    std::shuffle(indices.begin(), indices.end(), rng);
    indices.resize(tilesToCorrupt);

    // Bit-level corruption
    std::uniform_real_distribution<double> chance(0.0, 1.0);
    std::uniform_int_distribution<int> bitDist(0, 7);

    for (size_t tileIndex : indices)
    {
        size_t offset = start + tileIndex * TILE_SIZE;

        for (size_t b = 0; b < TILE_SIZE; ++b)
        {
            if (chance(rng) < intensity)
            {
                // Flip 1 random bit
                uint8_t mask = 1 << bitDist(rng);
                romData[offset + b] ^= mask;
            }
        }
    }
}