#pragma once
#include <AddressableDevice.hpp>

class GamePak : public AddressableDevice
{
public:
    AddressableDevice(const uint16_t size);
    virtual ~AddressableDevice() = default;

    uint8_t read(uint16_t addr, bool readOnly = false) const override;
    void write(uint16_t addr, uint8_t data) override;
};