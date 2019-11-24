#pragma once
#include <cstdint>
#include <memory>

class RamDevice;

class Bus
{
    std::shared_ptr<RamDevice> memory;

public:
    Bus();
    ~Bus() = default;
    void write(uint16_t addr, uint8_t data);
    uint8_t read(uint16_t addr, bool readOnly = false);
};