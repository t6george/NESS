#pragma once
#include <cstdint>
#include <vector>

class AddressableDevice;

class Bus
{
protected:

    struct AddrMapping
    {
        uint16_t base;
        uint16_t end;
        uint16_t mirrorSize;
    };

    std::vector<AddrMapping> mmu;

    virtual void initMmu() = 0;
    virtual AddressableDevice* delegate(uint16_t& addr) = 0;

public:
    Bus() = default;
    virtual ~Bus() = default;

    void write(uint16_t addr, uint8_t data);
    uint8_t read(uint16_t addr, bool readOnly = false);
};