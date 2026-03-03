#include <iostream>
#include <vector>
#include <string>
#include <sstream>

#include "RomReader.h"
#include "RomHeader.h"
#include "TileDecoder.h"
#include "ImageWriter.h"
#include "RomCorruptor.h"
#include <fstream>

using namespace std;

/*
    Decodes the entire ROM as raw 2bpp Game Boy tiles
    and writes them into a square PNG image.
*/
static void WriteFullRomImage(const vector<uint8_t> &romData, const string &title, size_t romSize)
{
    size_t tileCount = romData.size() / 16;

    int tilesPerRow = static_cast<int>(sqrt(tileCount));
    if (tilesPerRow <= 0)
        return;

    int width = tilesPerRow * 8;
    int height = (tileCount / tilesPerRow) * 8;

    PixelBuffer image = TileDecoder::DecodeAllTiles(
        romData,
        tilesPerRow,
        width,
        height);

    cout << "Writing output image...\n";
    string outputName = title + "_full.png";
    ImageWriter::WritePNG(outputName, image, width, height, title, romSize);
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
    if (!reader.isValid())
    {
        cout << "Invalid ROM\n";
        return 1;
    }

    const auto &romData = reader.getData();

    cout << "Loaded: " << filePath << "\n";
    cout << "Size: " << romData.size() << " bytes\n";

    RomHeader header(romData);
    cout << "Title: " << header.getTitle() << "\n";
    cout << "Cartridge Type: 0x" << hex
         << static_cast<int>(header.getCartridgeType()) << dec << "\n";
    cout << "ROM Size: " << header.getRomSizeBytes() / 1024 << " KB\n";
    cout << "Destination: "
         << (header.getDestination() == 0x00 ? "Japanese" : "Non-Japanese") << "\n";
    cout << "Checksum Valid: "
         << (header.isChecksumValid() ? "Yes" : "No") << "\n";
    cout << "Nintendo Logo Valid: "
         << (header.getLogoValid() ? "Yes" : "No") << "\n";

    WriteFullRomImage(romData, header.getTitle(), header.getRomSizeBytes());

    // Now let's corrupt!
    std::vector<uint8_t> romBase = reader.getData();
    // #TODO: make these user-configurable and more stable
    double corruptionPercent = 0.3;
    double corruptionIntensity = 0.15;
    RomCorruptor::corrupt(romBase, corruptionPercent, corruptionIntensity);

    string corruptedName = header.getTitle() + "_corrupted.gb";
    std::ofstream out(corruptedName, std::ios::binary);
    out.write(reinterpret_cast<const char *>(romBase.data()),
              romBase.size());
    out.close();

    return 0;
}