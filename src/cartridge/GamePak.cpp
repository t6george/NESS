#include <GamePak.hpp>
#include <fstream>

GamePak::GamePak(const std::string &fname, const uint16_t addrBase, const uint16_t addrEnd)
    : AddressableDevice::AddressableDevice(addrBase, addrEnd), prgBase{0x0000}
{
    std::ifstream in;
    in.open(fname, std::ifstream::binary);

    if (in.is_open())
    {
        in.read((char *)&header, sizeof(GameHeader));
        in.seekg(5, std::ios_base::cur);

        if (header.mapper1 & 0x04)
            in.seekg(512, std::ios_base::cur);

        mapperNum = ((header.mapper2 >> 4) << 4) | (header.mapper2 >> 4);

        uint8_t ftype = 1;

        if (ftype == 1)
        {
            chrBase = PRG_BANK_SIZE * header.prgBanks;
            contents.resize(chrBase + CHR_BANK_SIZE * header.chrBanks);
            in.read((char *)&contents, contents.size());
        }

        in.close();
    }
}

void GamePak::setByte(uint16_t addr, uint8_t data)
{
}

uint8_t GamePak::getByte(uint16_t addr, bool readOnly) const
{
    return 0;
}