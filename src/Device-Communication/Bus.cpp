#include <Bus.hpp>
#include <MMU.hpp>

Bus::Bus() : mmu{new MMU{}} {}

uint8_t Bus::read(uint16_t addr, bool readOnly) const
{
    return mmu->read(addr);
}

void Bus::write(uint16_t addr, uint8_t data) const
{
    mmu->write(addr, data);
}

void Bus::attachDevice(const uint16_t base, const uint16_t limit,
                       const uint16_t mirror, std::shared_ptr<AddressableDevice> device) const
{
    mmu->addEntry(AddressingInfo{
        .base = base,
        .limit = limit,
        .mirror = mirror,
        .device = device});
}