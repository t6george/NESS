#include <RicohRP2C02.hpp>
#include <Bus.hpp>
#include <HwConstants.hpp>

RicohRP2C02::RicohRP2C02() : cycles{0}, bus{new Bus{}} {}

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

void RicohRP2C02::addCartridge(std::shared_ptr<AddressableDevice> cart)
{
    bus->attachDevice(PPU::CARTRIDGE::Base,
                      PPU::CARTRIDGE::Limit,
                      PPU::CARTRIDGE::Mirror,
                      cart);
}

void RicohRP2C02::getPatternTable(const uint8_t tblIndex)
{
    uint16_t tile;
    uint8_t lsb, msb;

    for (uint16_t y = 0; y < 16; ++y)
    {
        for (uint16_t x = 0; x < 16; ++x)
        {
            tile = 256 * y + 16 * x;
            for (uint16_t row = 0; row < 8; ++row)
            {
                lsb = bus->read(tblIndex * 0x1000 + tile + row);
                msb = bus->read(tblIndex * 0x1000 + tile + row + 8);
            }
        }
    }
}

void RicohRP2C02::run()
{
    ++cycles;

    // if (cycles >= 341)
    // {
    //     cycles = 0;
    //     ++scanline;

    //     if (scanline >= 261)
    //     {
    //         scanline = -1;
    //         frameDrawn = true;

    //     }
    // }
}