#include <Bus.hpp>
#include <MMU.hpp>
#include <GamePad.hpp>

Bus::Bus(std::shared_ptr<GamePad> p1) : mmu{new MMU{}}, p1Controller{p1} {}

uint8_t Bus::read(uint16_t addr, bool readOnly)
{
    uint8_t data = 0x00;

    // if (addr >= 0x4016 && addr <= 0x4017)
    // {
    //     //addr & 0x0001
    //     data = (controllerState[addr & 0x0001] & 0x80) > 0;
    //     controllerState[addr & 0x0001] <<= 1;
    // }
    // else
    // {
    //     data = mmu->read(addr);
    // }

    switch (addr)
    {
    case 0x4016:
        data = p1Controller->readStateMSB();
        break;
    case 0x4017:
        break;
    default:
        data = mmu->read(addr);
        break;
    }

    return data;
}

void Bus::write(uint16_t addr, uint8_t data)
{
    switch (addr)
    {
    case 0x4016:
        p1Controller->writeButtonState();
        break;
    case 0x4017:
        break;
    default:
        mmu->write(addr, data);
        break;
    }
    // if (addr >= 0x4016 && addr <= 0x4017)
    // {
    //     controllerState[addr & 0x0001] = controller[addr & 0x0001];
    // }
    // else
    // {
    //     mmu->write(addr, data);
    // }
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