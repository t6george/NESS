#pragma once
#include <cstdint>

#include <HwConstants.hpp>

class Mapper
{
protected:
    const uint8_t prgBanks;
    const uint8_t chrBanks;

public:
    Mapper(const uint8_t prgBanks, const uint8_t chrBanks);
    virtual ~Mapper() = default;

    virtual bool translatePrgAddress(uint16_t &addr) const = 0;
    virtual bool translateChrAddress(uint16_t &addr) const = 0;
};