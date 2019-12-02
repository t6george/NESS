#pragma once
#include <AddressableDevice.hpp>
#include <string>
#include <iostream>

#define PRG_BANK_SIZE 16384
#define CHR_BANK_SIZE

class GamePak : public AddressableDevice
{
    struct GameHeader
    {
        char name[4];
        uint8_t prgBanks;
        uint8_t chrBanks;
        uint8_t mapper1;
        uint8_t mapper2;
        uint8_t prgRamSize;
        uint8_t tv1;
        uint8_t tv2;
        char unused[5];
    };

protected:
    virtual void setByte(uint16_t addr, uint8_t data) override;
    virtual uint8_t getByte(uint16_t addr, bool readOnly) const override;

public:
    GamePak(const std::string &fname, const uint16_t size, const uint16_t addrBase, const uint16_t addrEnd);
};