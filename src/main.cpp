#include <iostream>
#include <vector>
#include <string>
#include <sstream>

#include "rom_reader.h"
#include "rom_header.h"
#include "tile_decoder.h"
#include "image_writer.h"
#include "rom_corruptor.h"
#include <fstream>

using namespace std;

/*
    Decodes the entire ROM as raw 2bpp Game Boy tiles
    and writes them into a square PNG image.
*/
static void write_full_rom_image(const vector<uint8_t> &romData, const string &title, size_t romSize)
{
    size_t tileCount = romData.size() / 16;

    int tilesPerRow = static_cast<int>(sqrt(tileCount));
    if (tilesPerRow <= 0)
        return;

    int width = tilesPerRow * 8;
    int height = (tileCount / tilesPerRow) * 8;

    PixelBuffer image = TileDecoder::decode_all_tiles(
        romData,
        tilesPerRow,
        width,
        height);

    cout << "Writing output image...\n";
    string outputName = title + "_full.png";
    ImageWriter::write_png(outputName, image, width, height, title, romSize);
    cout << "Wrote " << outputName << "\n";
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cout << "Usage: gb-file-reader <rom-file> [bank]\n";
        return 1;
    }

    string filePath = argv[1];

    RomReader reader(filePath);
    if (!reader.is_valid())
    {
        cout << "Invalid ROM\n";
        return 1;
    }

    const auto &romData = reader.get_data();

    cout << "Loaded: " << filePath << "\n";
    cout << "Size: " << romData.size() << " bytes\n";

    RomHeader header(romData);
    cout << "Title: " << header.get_title() << "\n";
    cout << "Cartridge Type: 0x" << hex
         << static_cast<int>(header.get_cartridge_type()) << dec << "\n";
    cout << "ROM Size: " << header.get_rom_size_bytes() / 1024 << " KB\n";
    cout << "Destination: "
         << (header.get_destination() == 0x00 ? "Japanese" : "Non-Japanese") << "\n";
    cout << "Checksum Valid: "
         << (header.is_checksum_valid() ? "Yes" : "No") << "\n";
    cout << "Nintendo Logo Valid: "
         << (header.get_logo_valid() ? "Yes" : "No") << "\n";

    write_full_rom_image(romData, header.get_title(), header.get_rom_size_bytes());

    // Now let's corrupt!
    std::vector<uint8_t> romBase = reader.get_data();
    // #TODO: make these user-configurable and more stable
    double corruptionPercent = 0.3;
    double corruptionIntensity = 0.15;
    RomCorruptor::corrupt(romBase, corruptionPercent, corruptionIntensity);

    string corruptedName = header.get_title() + "_corrupted.gb";
    std::ofstream out(corruptedName, std::ios::binary);
    out.write(reinterpret_cast<const char *>(romBase.data()),
              romBase.size());
    out.close();

    return 0;
}