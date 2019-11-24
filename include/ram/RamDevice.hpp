#pragma once
#include <vector>
#include <cstdint>

class RamDevice
{
    const uint16_t addrBase;
    std::vector<uint8_t> contents;

public:
    RamDevice(const uint16_t addrBase, const uint16_t addrRange);
    ~RamDevice() = default;
    void write(uint16_t addr, uint8_t data);
    uint8_t read(uint16_t addr, bool readOnly = false) const;
};