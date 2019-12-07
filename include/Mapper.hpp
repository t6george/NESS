#pragma once
#include <cstdint>

class Mapper
{
protected:
    const uint8_t prgBanks;
    const uint8_t chrBanks;

public:
    Mapper(const uint8_t prgBanks, const uint8_t chrBanks);
    virtual ~Mapper() = default;

    virtual uint16_t translatePrgAddress(uint16_t addr) const = 0;
    virtual uint16_t translateChrAddress(uint16_t addr) const = 0;
};