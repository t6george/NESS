#include <RicohRP2C02.hpp>
#include <Bus.hpp>

RicohRP2C02::RicohRP2C02(const uint16_t size, const uint16_t addrBase, const uint16_t addrEnd)
    : AddressableDevice::AddressableDevice{{size}, addrBase, addrEnd}, bus{new Bus{}}
{
    //bus->attachDevice();
}

void RicohRP2C02::setByte(uint16_t addr, uint8_t data)
{
    switch (addr)
    {
    case 0x0000:
        break;
    case 0x0001:
        break;
    case 0x0002:
        break;
    case 0x0003:
        break;
    case 0x0004:
        break;
    case 0x0005:
        break;
    case 0x0006:
        break;
    case 0x0007:
        break;
    default:
        break;
    }
}

uint8_t RicohRP2C02::getByte(uint16_t addr, bool readOnly) const
{
    switch (addr)
    {
    case 0x0000:
        break;
    case 0x0001:
        break;
    case 0x0002:
        break;
    case 0x0003:
        break;
    case 0x0004:
        break;
    case 0x0005:
        break;
    case 0x0006:
        break;
    case 0x0007:
        break;
    default:
        break;
    }
    return 0;
}

void RicohRP2C02::write(uint16_t addr, uint8_t data)
{
    addr &= 0x3FFF;
}

uint8_t RicohRP2C02::read(uint16_t addr) const
{
    addr &= 0x3FFF;
    return 0;
}

void RicohRP2C02::addCartridge(std::shared_ptr<AddressableDevice> cart)
{
    bus->attachDevice(cart);
}

void RicohRP2C02::run()
{
}