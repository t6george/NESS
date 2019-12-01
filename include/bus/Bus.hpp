#pragma once
#include <cstdint>
#include <memory>

class AddressableDevice;
class RicohRP2C02;

class Bus
{
    std::shared_ptr<AddressableDevice> memory;
    std::shared_ptr<AddressableDevice> ppu;

public:
    Bus();
    ~Bus() = default;
    void write(uint16_t addr, uint8_t data);
    uint8_t read(uint16_t addr, bool readOnly = false);
};