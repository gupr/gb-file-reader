#include "RomReader.h"

#include <fstream>
#include <iostream>

namespace
{
    constexpr std::size_t MIN_ROM_SIZE = 32768;
}

// ================================
// Constructor
// ================================

RomReader::RomReader(const std::string &filePath)
{
    valid = false;
    loadFile(filePath);
}

// ================================
// Public Methods
// ================================

bool RomReader::isValid() const
{
    return valid;
}

const std::vector<uint8_t> &RomReader::getData() const
{
    return romData;
}

// ================================
// Private Methods
// ================================

// Load the .gb file with the given filePath
void RomReader::loadFile(const std::string &filePath)
{
    // Open the file
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open())
    {
        valid = false;
        return;
    }

    // Get the size of the file.
    file.seekg(0, std::ios::end);
    std::streamsize fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    if (fileSize < MIN_ROM_SIZE)
    {
        valid = false;
        return;
    }

    romData.resize(fileSize);

    // Read into binary data
    file.read(reinterpret_cast<char *>(romData.data()), fileSize);

    if (!file)
    {
        valid = false;
        return;
    }

    valid = true;
}