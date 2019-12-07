#include <Mapper000.hpp>

Mapper000::Mapper000(const uint8_t prgBanks, const uint8_t chrBanks)
    : Mapper::Mapper(prgBanks, chrBanks) {}

bool Mapper000::translatePrgAddress(uint16_t &addr) const
{
    bool addressable = false;
    addr += CPU::CARTRIDGE::Base;

    if (M000::PRG::Base <= addr && M000::PRG::Limit >= addr)
    {
        addressable = true;
        addr = (addr - M000::PRG::Base) & (prgBanks > 1 ? 0x7FFF : 0x3FFF);
    }

    return addressable;
}

bool Mapper000::translateChrAddress(uint16_t &addr) const
{
    bool addressable = false;
    addr += PPU::CARTRIDGE::Base;

    if (M000::PRG::Base <= addr && M000::PRG::Limit >= addr)
    {
        addressable = true;
    }

    return addressable;
}