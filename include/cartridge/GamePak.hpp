#pragma once
#include <string>
#include <vector>

#include <AddressableDevice.hpp>
#include <Mapper.hpp>

#define PRG_BANK_SIZE 0x4000
#define CHR_BANK_SIZE 0x2000

class GamePak : public AddressableDevice
{
    enum ActiveMemory
    {
        PRG,
        CHR,
    };

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

    uint8_t mapperNum;

    std::vector<uint8_t> prg;
    std::vector<uint8_t> chr;

    GameHeader header;

    ActiveMemory mem;

    void setByte(uint16_t addr, uint8_t data) override;
    uint8_t getByte(uint16_t addr, bool readOnly) const override;

public:
    GamePak(const std::string &fname);

    uint8_t read(uint16_t addr, uint16_t mirror, bool readOnly = false) override;
    void write(uint16_t addr, uint16_t mirror, uint8_t data) override;
};