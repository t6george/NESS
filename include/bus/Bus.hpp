#pragma once
#include <cstdint>
#include <memory>

class MemoryDevice;
class RicohRP2C02;

class Bus
{
    std::shared_ptr<MemoryDevice> memory;
    std::shared_ptr<MemoryDevice> ppu;

public:
    Bus();
    ~Bus() = default;
    void write(uint16_t addr, uint8_t data);
    uint8_t read(uint16_t addr, bool readOnly = false);
};