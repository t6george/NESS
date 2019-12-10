#include <RicohRP2C02.hpp>
#include <Bus.hpp>
#include <HwConstants.hpp>

RicohRP2C02::RicohRP2C02()
    : cycles{0}, bus{new Bus{}},
      colors{0x666666, 0x002A88, 0x1412A7, 0x3B00A4,
             0x5C007E, 0x6E0040, 0x6C0600, 0x561D00,
             0x333500, 0x0B4800, 0x005200, 0x004F08,
             0x00404D, 0x000000, 0x000000, 0x000000,
             0xADADAD, 0x155FD9, 0x4240FF, 0x7527FE,
             0xA01ACC, 0xB71E7B, 0xB53120, 0x994E00,
             0x6B6D00, 0x388700, 0x0C9300, 0x008F32,
             0x007C8D, 0x000000, 0x000000, 0x000000,
             0xFFFEFF, 0x64B0FF, 0x9290FF, 0xC676FF,
             0xF36AFF, 0xFE6ECC, 0xFE8170, 0xEA9E22,
             0xBCBE00, 0x88D800, 0x5CE430, 0x45E082,
             0x48CDDE, 0x4F4F4F, 0x000000, 0x000000,
             0xFFFEFF, 0xC0DFFF, 0xD3D2FF, 0xE8C8FF,
             0xFBC2FF, 0xFEC4EA, 0xFECCC5, 0xF7D8A5,
             0xE4E594, 0xCFEF96, 0xBDF4AB, 0xB3F3CC,
             0xB5EBF2, 0xB8B8B8, 0x000000, 0x000000} {}

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