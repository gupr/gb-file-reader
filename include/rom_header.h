#ifndef ROMHEADER_H
#define ROMHEADER_H

#include <vector>
#include <string>
#include <cstdint>

class RomHeader
{
public:
    explicit RomHeader(const std::vector<uint8_t> &romData);

    std::string get_title() const;
    uint8_t get_cartridge_type() const;
    uint8_t get_rom_size_code() const;
    size_t get_rom_size_bytes() const;
    uint8_t get_destination() const;
    bool is_checksum_valid() const;
    bool get_logo_valid() const;

private:
    void parse_header(const std::vector<uint8_t> &romData);
    bool validate_checksum(const std::vector<uint8_t> &romData);
    bool validate_nintendo_logo(const std::vector<uint8_t> &romData);

    std::string title;
    uint8_t cartridgeType;
    uint8_t romSizeCode;
    uint8_t destination;
    uint8_t headerChecksum;
    bool checksumValid;
    bool logoValid;
};

#endif