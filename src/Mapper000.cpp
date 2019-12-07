#include <Mapper000.hpp>

Mapper000::Mapper000(const uint8_t prgBanks, const uint8_t chrBanks)
    : Mapper::Mapper(prgBanks, chrBanks) {}

uint16_t Mapper000::translatePrgAddress(uint16_t addr) const
{
    return 0;
}

uint16_t Mapper000::translateChrAddress(uint16_t addr) const
{
    return 0;
}