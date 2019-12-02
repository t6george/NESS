#pragma once
#include <AddressableDevice.hpp>
#include <string>

#define PRG_BANK_SIZE 0x4000
#define CHR_BANK_SIZE 0x2000

class GamePak : public AddressableDevice
{
protected:
    typedef struct
    {
        uint8_t name[4];
        uint8_t prgBanks;
        uint8_t chrBanks;
        uint8_t mapper1;
        uint8_t mapper2;
        uint8_t prgRamSize;
        uint8_t tv1;
        uint8_t tv2;
    } GameHeader;

    GameHeader header;

    virtual void setByte(uint16_t addr, uint8_t data) override;
    virtual uint8_t getByte(uint16_t addr, bool readOnly) const override;

public:
    GamePak(const std::string &fname, const uint16_t addrBase, const uint16_t addrEnd);
};