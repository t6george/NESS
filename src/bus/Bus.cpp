#include <AddressableDevice.hpp>
#include <Bus.hpp>

void Bus::write(uint16_t addr, uint8_t data)
{
    for (const auto dev : addressableDevices)
    {
        if (dev->write(addr, data))
            break;
    }
}

uint8_t Bus::read(uint16_t addr, bool readOnly)
{
    uint8_t data = 0x00;

    for (const auto dev : addressableDevices)
    {
        data = dev->read(addr, readOnly);

        if (data != 0)
            break;
    }
    return data;
}

void Bus::attachDevice(std::shared_ptr<AddressableDevice> device)
{
    addressableDevices.emplace_back(device);
}