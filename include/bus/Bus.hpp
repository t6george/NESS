#pragma once
#include <cstdint>
#include <memory>

class AddressableDevice;

class Bus
{
    std::shared_ptr<AddressableDevice> memory;

public:
    Bus();
    ~Bus() = default;
    void write(uint16_t addr, uint8_t data);
    uint8_t read(uint16_t addr, bool readOnly = false);
};