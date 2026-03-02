#include <iostream>
#include <vector>
#include <string>
#include "RomReader.h"
#include "RomHeader.h"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cout << "Usage: gb-file-reader <rom-file>\n";
        return 1;
    }

    std::string filePath = argv[1];

    cout << "GB FILE READER STARTED" << endl;
    cout << "------------------------\n";

    RomReader reader(filePath);

    if (!reader.isValid())
    {
        cout << "Invalid ROM!\n";
        return 1;
    }

    cout << "Loaded " << filePath << " successfully.\n";
    cout << "Size: " << reader.getData().size() << " bytes\n";

    RomHeader header(reader.getData());
    cout << "Title: " << header.getTitle() << endl;
    cout << "Cartridge Type: 0x" << hex << static_cast<int>(header.getCartridgeType()) << dec << endl;
    cout << "ROM Size: "
         << header.getRomSizeBytes() / 1024
         << " KB\n";

    header.getDestination() == 0x00 ? cout << "Destination: Japanese\n" : cout << "Destination: Non-Japanese\n";
    cout << "Checksum Valid: " << (header.isChecksumValid() ? "Yes" : "No") << endl;
    cout << "Nintendo Logo Valid: " << (header.getLogoValid() ? "Yes" : "No") << endl;

    return 0;
}
