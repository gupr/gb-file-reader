#ifndef ROMREADER_H
#define ROMREADER_H

#include <vector>
#include <string>
#include <cstdint>

class RomReader
{
public:
    explicit RomReader(const std::string &filePath);

    bool isValid() const;
    const std::vector<uint8_t> &getData() const;

private:
    void loadFile(const std::string &filePath);

    std::vector<uint8_t> romData;
    bool valid;
};

#endif