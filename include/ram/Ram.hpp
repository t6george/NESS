#pragma once
#include <AddressableDevice.hpp>

/*
 * NES RAM's address space is 8KB, but it is only 2KB in size
 * This is achieved through 3 additional 2KB regions that mirror
 * the first 2KB region. To achieve this, the modulo of the address
 * and physical RAM size is the actual address being accessed
 */

class Ram : public AddressableDevice
{
    std::vector<uint8_t> memory;

public:
    AddressableDevice(const uint16_t size);
    virtual ~AddressableDevice() = default;

    uint8_t read(uint16_t addr, bool readOnly = false) const override;
    void write(uint16_t addr, uint8_t data) override;
};