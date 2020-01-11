#include <Bus.hpp>
#include <MMU.hpp>
#include <iostream>

Bus::Bus() : mmu{new MMU{}} {}

uint8_t Bus::read(uint16_t addr, bool readOnly)
{
    uint8_t data = 0x00;

    if (addr >= 0x4016 && addr <= 0x4017)
    {
        //addr & 0x0001
        data = (controllerState[0] & 0x80) > 0;
        controllerState[0] <<= 1;
    }
    else
    {
        data = mmu->read(addr);
    }

    return data;
}

void Bus::write(uint16_t addr, uint8_t data)
{
    if (addr >= 0x4016 && addr <= 0x4017)
    {
        controllerState[0] = controller[0];
    }
    else
    {
        mmu->write(addr, data);
    }
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