#pragma once
#include <vector>
#include <cstdint>

class RomCorruptor
{
public:
    // percent: 0–100
    // intensity: 0.0–1.0
    // seed: deterministic if provided
    static void corrupt(std::vector<uint8_t> &romData,
                        double percent,
                        double intensity = 0.25,
                        uint32_t seed = 0);
};