#pragma once
#include <cstdint>
#include <memory>

class MMU;
class AddressableDevice;

class Bus
{
    std::shared_ptr<MMU> mmu;

public:
    Bus();
    ~Bus() = default;
    void write(uint16_t addr, uint8_t data);
    uint8_t read(uint16_t addr, bool readOnly = false);
    void attachDevice(const uint16_t base, const uint16_t limit,
                      const uint16_t mirror, std::shared_ptr<AddressableDevice> device) const;

    uint8_t controller[2];
    uint8_t controllerState[2];
};