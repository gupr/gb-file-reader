#include "RomHeader.h"
#include <stdexcept>

// Game Boy ROM header offsets
constexpr int TITLE_START = 0x134;
constexpr int TITLE_LENGTH = 16;
constexpr int CARTRIDGE_TYPE_OFFSET = 0x147;
constexpr int ROM_SIZE_OFFSET = 0x148;
constexpr int HEADER_CHECKSUM_OFFSET = 0x14D;
constexpr int CHECKSUM_START = 0x134;
constexpr int CHECKSUM_END = 0x14C;

static const uint8_t NINTENDO_LOGO[48] = {
    0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B,
    0x03, 0x73, 0x00, 0x83, 0x00, 0x0C, 0x00, 0x0D,
    0x00, 0x08, 0x11, 0x1F, 0x88, 0x89, 0x00, 0x0E,
    0xDC, 0xCC, 0x6E, 0xE6, 0xDD, 0xDD, 0xD9, 0x99,
    0xBB, 0xBB, 0x67, 0x63, 0x6E, 0x0E, 0xEC, 0xCC,
    0xDD, 0xDC, 0x99, 0x9F, 0xBB, 0xB9, 0x33, 0x3E};

constexpr size_t LOGO_OFFSET = 0x0104;

RomHeader::RomHeader(const std::vector<uint8_t> &data)
{
    if (data.size() < 0x0150)
        throw std::runtime_error("ROM too small to contain valid header");

    parseHeader(data);
}

void RomHeader::parseHeader(const std::vector<uint8_t> &romData)
{
    // Title: 0x0134–0x0143
    for (size_t i = 0x0134; i <= 0x0143; ++i)
    {
        if (romData[i] == 0x00)
            break;

        title += static_cast<char>(romData[i]);
    }

    cartridgeType = romData[CARTRIDGE_TYPE_OFFSET];
    romSizeCode = romData[ROM_SIZE_OFFSET];
    destination = romData[0x14A];
    checksumValid = validateChecksum(romData);
    logoValid = validateNintendoLogo(romData);
}

// Validate the checksum of the header. The checksum is calculated by summing bytes 0x0134 to 0x014C
bool RomHeader::validateChecksum(const std::vector<uint8_t> &romData)
{
    if (romData.size() <= HEADER_CHECKSUM_OFFSET)
        return false;

    uint8_t checksum = 0;
    for (size_t i = CHECKSUM_START; i <= CHECKSUM_END; ++i)
    {
        checksum = checksum - romData[i] - 1;
    }
    return checksum == romData[HEADER_CHECKSUM_OFFSET];
}

std::string RomHeader::getTitle() const
{
    return title;
}

uint8_t RomHeader::getCartridgeType() const
{
    return cartridgeType;
}

uint8_t RomHeader::getRomSizeCode() const
{
    return romSizeCode;
}

size_t RomHeader::getRomSizeBytes() const
{
    if (romSizeCode <= 7)
        return 32 * 1024ULL << romSizeCode;

    return 0; // unknown
}

uint8_t RomHeader::getDestination() const
{
    return destination;
}

bool RomHeader::isChecksumValid() const
{
    return checksumValid;
}

bool RomHeader::validateNintendoLogo(const std::vector<uint8_t> &romData)
{
    for (size_t i = 0; i < sizeof(NINTENDO_LOGO); ++i)
    {
        if (romData[LOGO_OFFSET + i] != NINTENDO_LOGO[i])
            return false;
    }
    return true;
}

bool RomHeader::getLogoValid() const
{
    return logoValid;
}