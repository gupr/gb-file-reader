#ifndef ROMHEADER_H
#define ROMHEADER_H

#include <vector>
#include <string>
#include <cstdint>

class RomHeader
{
public:
    explicit RomHeader(const std::vector<uint8_t> &romData);

    std::string getTitle() const;
    uint8_t getCartridgeType() const;
    uint8_t getRomSizeCode() const;
    size_t getRomSizeBytes() const;
    uint8_t getDestination() const;
    bool isChecksumValid() const;
    bool getLogoValid() const;

private:
    void parseHeader(const std::vector<uint8_t> &romData);
    bool validateChecksum(const std::vector<uint8_t> &romData);
    bool validateNintendoLogo(const std::vector<uint8_t> &romData);

    std::string title;
    uint8_t cartridgeType;
    uint8_t romSizeCode;
    uint8_t destination;
    uint8_t headerChecksum;
    bool checksumValid;
    bool logoValid;
};

#endif