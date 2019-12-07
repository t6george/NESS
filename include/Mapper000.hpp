#pragma once
#include <Mapper.hpp>

namespace M000
{

namespace PRG
{
const uint16_t Base = 0x8000;
const uint16_t Limit = 0xFFFF;
} // namespace PRG

namespace CHR
{
const uint16_t Base = 0x0000;
const uint16_t Limit = 0x1FFF;
}; // namespace CHR

}; // namespace M000

class Mapper000 : public Mapper
{
public:
    Mapper000(const uint8_t prgBanks, const uint8_t chrBanks);
    virtual ~Mapper000() = default;

    bool translatePrgAddress(uint16_t &addr) const override;
    bool translateChrAddress(uint16_t &addr) const override;
};