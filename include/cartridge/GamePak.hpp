#pragma once
#include <string>
#include <vector>
#include <memory>

#include <AddressableDevice.hpp>
#include <Mapper.hpp>
#include <HwConstants.hpp>

class GamePak : public AddressableDevice
{
public:
    enum MirrorMode
    {
        VERTICAL,
        HORIZONTAL
    };

    GamePak(const std::string &fname);
    GamePak::MirrorMode getMirrorMode() const;

private:
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
        uint8_t unused[5];
    } GameHeader;

    std::unique_ptr<Mapper> mapper;

    std::vector<uint8_t> prg;
    std::vector<uint8_t> chr;

    GameHeader header;
    ActiveMemory mem;
    MirrorMode mMode;

    void setByte(uint16_t addr, uint8_t data) override;
    uint8_t getByte(uint16_t addr, bool readOnly) override;

    uint16_t mirrorAddress(uint16_t addr, uint16_t mirror) override;
    void parseFile(const std::string &fname);
};