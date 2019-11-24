#pragma once
#include <array>
#include <cstdint>

class RamDevice
{
    constexpr uint16_t addrBase;
    constexpr uint16_t addrRange;
    std::array<uint8_t, addrRange> contents;

public:
    RamDevice(constexpr uint16_t addrBase, constexpr uint16_t addrRange);
    ~RamDevice() = default;
    void write(uint16_t addr, uint8_t, data);
    uint8_t read(uint16_t addr, bool readOnly = false) const;
};