#include <fstream>
#include <cassert>

#include <GamePak.hpp>
#include <Mapper000.hpp>
#include <SDL2/SDL.h>

GamePak::GamePak(const std::string &fname) : mem{PRG}
{
    parseFile(fname);
}

void GamePak::parseFile(const std::string &fname)
{
    std::ifstream in;
    in.open(fname, std::ifstream::binary);

    if (in.is_open())
    {
        in.read((char *)&header, sizeof(GameHeader));

        if (header.mapper1 & 0x04)
            in.seekg(0x200, std::ios_base::cur);

        uint8_t mapperNum = (header.mapper2 & 0xFFF0) | (header.mapper1 >> 0x4);
        mMode = (header.mapper1 & 0x01) ? VERTICAL : HORIZONTAL;

        uint8_t ftype = 1;

        if (ftype == 1)
        {
            header.chrBanks = header.chrBanks == 0 ? 0x1 : header.chrBanks;
            prg.resize(CARTRIDGE::PrgBankSize * header.prgBanks);
            chr.resize(CARTRIDGE::ChrBankSize * header.chrBanks);

            in.read((char *)prg.data(), prg.size());
            in.read((char *)chr.data(), chr.size());

            // SDL_Log("PRG Dump\n");
            // for (int i = 0; i < prg.size(); ++i)
            // {
            //     SDL_Log("prg %X: %X ", i, prg[i]);
            // }

            // SDL_Log("\n\nCHR Dump\n");
            // for (int i = 0; i < chr.size(); ++i)
            // {
            //     SDL_Log("chr %X: %X ", i, chr[i]);
            // }
        }

        switch (mapperNum)
        {
        case 0:
            mapper.reset(new Mapper000(header.prgBanks, header.chrBanks));
            break;
        default:
            // Unsupported Mapper Type
            assert(false);
            break;
        }

        in.close();
    }
    else
    {
        assert(false);
    }
}
#include <iostream>
uint8_t GamePak::getByte(uint16_t addr, bool readOnly)
{
    uint8_t data = 0x00;
    // if (mem == PRG)
    //     std::cout << "PRG Mode" << std::endl;
    // else if (mem == CHR)
    //     std::cout << "CHR Mode" << std::endl;

    if (mem == PRG && mapper->translatePrgAddress(addr))
    {
        data = prg[addr];
        // std::cout << "rd prg " << std::hex << static_cast<int>(addr) << " " << static_cast<int>(data) << std::endl;
    }
    else if (mem == CHR && mapper->translateChrAddress(addr))
    {
        data = chr[addr];
        // std::cout << "rd chr " << std::hex << static_cast<int>(addr) << " " << static_cast<int>(data) << std::endl;
    }

    return data;
}

void GamePak::setByte(uint16_t addr, uint8_t data)
{
    // if (mem == PRG)
    //     std::cout << "PRG Mode" << std::endl;
    // else if (mem == CHR)
    //     std::cout << "CHR Mode" << std::endl;
    if (mem == PRG && mapper->translatePrgAddress(addr))
    {
        // std::cout << "wr prg " << std::hex << static_cast<int>(addr) << " " << static_cast<int>(data) << std::endl;
        prg[addr] = data;
    }
    else if (mem == CHR && mapper->translateChrAddress(addr))
    {
        // std::cout << "wr chr " << std::hex << static_cast<int>(addr) << " " << static_cast<int>(data) << std::endl;
        chr[addr] = data;
    }
}

uint16_t GamePak::mirrorAddress(uint16_t addr, uint16_t mirror)
{
    if (mirror == CPU::CARTRIDGE::Mirror)
    {
        mem = PRG;
    }
    else if (mirror == PPU::CARTRIDGE::Mirror)
    {
        mem = CHR;
    }

    return addr;
}

GamePak::MirrorMode GamePak::getMirrorMode() const
{
    return mMode;
}