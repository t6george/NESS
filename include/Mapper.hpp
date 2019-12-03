#pragma once
#include <cstdint>

class Mapper
{
    const uint8_t prgBanks;
    const uint8_t chrBanks;

public:
    Mapper(const uint8_t prgBanks, const uint8_t chrBanks);
    virtual ~Mapper() = default;
};