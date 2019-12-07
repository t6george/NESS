#pragma once
#include <Mapper.hpp>

class Mapper000 : public Mapper
{
public:
    Mapper000(const uint8_t prgBanks, const uint8_t chrBanks);
    virtual ~Mapper000() = default;

    uint16_t translatePrgAddress(uint16_t addr) const override;
    uint16_t translateChrAddress(uint16_t addr) const override;
};