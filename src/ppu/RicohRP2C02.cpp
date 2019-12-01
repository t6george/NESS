#include <RicohRP2C02.hpp>
#include <Bus.hpp>

RicohRP2C02::RicohRP2C02(const uint16_t size, const uint16_t addrBase, const uint16_t addrEnd): 
AddressableDevice::AddressableDevice{size, addrBase, addrEnd}, bus{new Bus{}} {}

void RicohRP2C02::setByte(uint16_t addr, uint8_t data)
{
    switch(addr)
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
    switch(addr)
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