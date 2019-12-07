#include <GamePak.hpp>
#include <fstream>
#include <HwConstants.hpp>

GamePak::GamePak(const std::string &fname) : mem{CHR}
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
            prg.resize(PRG_BANK_SIZE * header.prgBanks);
            chr.resize(CHR_BANK_SIZE * header.chrBanks);

            in.read((char *)&prg, prg.size());
            in.read((char *)&chr, chr.size());
        }

        in.close();
    }
}

uint8_t GamePak::getByte(uint16_t addr, bool readOnly) const
{
    uint8_t data = 0x00;

    if (mem == PRG)
    {
        addr = mapper->translatePrgAddress(addr);
        data = prg[addr];
    }
    else if (mem == CHR)
    {
        addr = mapper->translateChrAddress(addr);
        data = chr[addr];
    }

    return data;
}

void GamePak::setByte(uint16_t addr, uint8_t data)
{
    if (mem == PRG)
    {
        addr = mapper->translatePrgAddress(addr);
        prg[addr] = data;
    }
    else if (mem == CHR)
    {
        addr = mapper->translateChrAddress(addr);
        chr[addr] = data;
    }
}

uint8_t GamePak::read(uint16_t addr, uint16_t mirror, bool readOnly)
{
    if (mirror == CPU::CARTRIDGE::Mirror)
    {
        mem = PRG;
    }
    else if (mirror == PPU::CARTRIDGE::Mirror)
    {
        mem = CHR;
    }
    return getByte(addr, readOnly);
}

void GamePak::write(uint16_t addr, uint16_t mirror, uint8_t data)
{
    if (mirror == CPU::CARTRIDGE::Mirror)
    {
        mem = PRG;
    }
    else if (mirror == PPU::CARTRIDGE::Mirror)
    {
        mem = CHR;
    }
    setByte(addr, data);
}