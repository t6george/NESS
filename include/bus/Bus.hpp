#pragma once
#include <cstdint>
#include <memory>
#include <vector>

class AddressableDevice;

class Bus
{
    std::vector<std::shared_ptr<AddressableDevice>> addressableDevices;

public:
    Bus() = default;
    ~Bus() = default;
    void write(uint16_t addr, uint8_t data);
    uint8_t read(uint16_t addr, bool readOnly = false);
    void attachDevice(std::shared_ptr<AddressableDevice> device);
};