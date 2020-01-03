#include <MMU.hpp>
#include <AddressableDevice.hpp>

void MMU::addEntry(const AddressingInfo entry)
{
    virtToPhys.emplace_back(entry);
}

uint8_t MMU::read(uint16_t addr)
{
    uint8_t data = 0x00;

    // if (addr >= 0x4016 && addr <= 0x4017)
    // {
    //     data = (controller_state[addr & 0x0001] & 0x80) > 0;
    //     controller_state[addr & 0x0001] <<= 1;
    //     return data;
    // }

    for (size_t i = 0; i < virtToPhys.size(); ++i)
    {
        if (addr <= virtToPhys[i].limit && addr >= virtToPhys[i].base)
        {
            addr -= virtToPhys[i].base;
            data = virtToPhys[i].device->read(addr, virtToPhys[i].mirror);
            break;
        }
    }

    return data;
}

void MMU::write(uint16_t addr, uint8_t data)
{
    // if (addr >= 0x4016 && addr <= 0x4017)
    // {
    //     controller[0] = 1;
    //     controller_state[addr & 0x0001] = controller[addr & 0x0001];
    //     return;
    // }

    for (size_t i = 0; i < virtToPhys.size(); ++i)
    {
        if (addr <= virtToPhys[i].limit && addr >= virtToPhys[i].base)
        {
            addr -= virtToPhys[i].base;
            virtToPhys[i].device->write(addr, virtToPhys[i].mirror, data);
            break;
        }
    }
}