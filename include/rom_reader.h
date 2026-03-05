#ifndef ROMREADER_H
#define ROMREADER_H

#include <vector>
#include <string>
#include <cstdint>

class RomReader
{
public:
    explicit RomReader(const std::string &filePath);

    bool is_valid() const;
    const std::vector<uint8_t> &get_data() const;

private:
    void load_file(const std::string &filePath);

    std::vector<uint8_t> romData;
    bool valid;
};

#endif