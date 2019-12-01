#pragma once

class AddressableDevice
{

public:
    AddressableDevice() = default;
    virtual ~AddressableDevice() = default;

    virtual uint8_t read(uint16_t addr, bool readOnly = false) const = 0;
    virtual void write(uint16_t addr, uint8_t data) = 0;
};