#pragma once
#include <cstdint>
#include <memory>

class MMU;
class AddressableDevice;

class Bus
{
    std::shared_ptr<MMU> mmu;

public:
    Bus() = default;
    ~Bus() = default;
    void write(uint16_t addr, uint8_t data) const;
    uint8_t read(uint16_t addr, bool readOnly = false) const;
    void attachDevice(const uint16_t base, const uint16_t limit,
                      const uint16_t mirror, std::shared_ptr<AddressableDevice> device) const;
};