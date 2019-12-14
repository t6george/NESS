#include <cassert>

#include <RicohRP2C02.hpp>
#include <Bus.hpp>
#include <Vram.hpp>
#include <PaletteRam.hpp>
#include <HwConstants.hpp>
#include <GamePak.hpp>

RicohRP2C02::RicohRP2C02() : cycles{0}, addrLatch{0x1}, ppuAddr{0x0000}, 
      dataBuffer{0x00}, scanline{0}, requestCpuNmi{false}, bus{new Bus{}},
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
             0xB5EBF2, 0xB8B8B8, 0x000000, 0x000000},
             frameBuffer{std::vector<uint32_t>(
                 DISPLAY::Width * DISPLAY::Height, 
                 DISPLAY::PixelOpacity)} 
{
    bus->attachDevice(PPU::PALETTE::Base,
                      PPU::PALETTE::Limit,
                      PPU::PALETTE::Mirror,
                      std::shared_ptr<AddressableDevice>(
                          new PaletteRam(PPU::PALETTE::Size)));
}

uint8_t RicohRP2C02::getByte(uint16_t addr, bool readOnly)
{
    uint8_t data = 0x00;

    switch (addr)
    {
    case 0x0000:
        break;
    case 0x0001:
        break;
    case 0x0002:
        statusRegister.scratch = dataBuffer & 0x1F;
        data = statusRegister.raw;
        statusRegister.vertical_blank = 0;
        addrLatch = 0x1;
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
        data = dataBuffer;
        dataBuffer = localRead(ppuAddr);

        if(addr >= PPU::PALETTE::Base && addr <= PPU::PALETTE::Limit)
        {
            data = dataBuffer;
        }
        ++ppuAddr;
        break;
    default:
        break;
    }

    return data;
}

void RicohRP2C02::setByte(uint16_t addr, uint8_t data)
{
    switch (addr)
    {
    case 0x0000:
        controlRegister.raw = data;
        break;
    case 0x0001:
        maskRegister.raw = data;
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
        ppuAddr = (ppuAddr & ~(static_cast<uint16_t>(0xFF) << (addrLatch * 0x8))) 
            | (static_cast<uint16_t>(data) << (addrLatch * 0x8));
        addrLatch ^= 0x1;
        break;
    case 0x0007:
        localWrite(ppuAddr++, data);
        break;
    default:
        break;
    }
}

void RicohRP2C02::addCartridge(std::shared_ptr<AddressableDevice> cart)
{
    GamePak* game = nullptr;
    assert(game = dynamic_cast<GamePak*>(cart.get()));

    bus->attachDevice(PPU::CARTRIDGE::Base,
                      PPU::CARTRIDGE::Limit,
                      PPU::CARTRIDGE::Mirror,
                      cart);

    bus->attachDevice(PPU::VRAM::Base,
                      PPU::VRAM::Limit,
                      PPU::VRAM::Mirror,
                      std::shared_ptr<AddressableDevice>(
                          new VRam(PPU::VRAM::Size, game->getMirrorMode())));
}

void RicohRP2C02::updateFrameBuffer(const uint8_t tblIndex)
{
    uint16_t chrBase = tblIndex * PPU::CARTRIDGE::CHR::PartitionSize, pixel;
    uint8_t lsBits, msBits;

    for (uint16_t offset = 0; offset < PPU::CARTRIDGE::CHR::NumTiles; offset += 2)
    {
        for (uint16_t row = 0; row < 0x8; ++row)
        {
            lsBits = localRead(chrBase + row);
            msBits = localRead(chrBase + row + 0x8);

            for (uint16_t col = 0; col < 0x8; ++col)
            {
                pixel = ((lsBits & 0x80) >> 0x7) | ((msBits & 0x80) >> 0x6);
                lsBits <<= 0x1;
                msBits <<= 0x1;
                frameBuffer[offset + row * DISPLAY::Width + col] |= 
                    getRgb(tblIndex, pixel);
            }
        }
    }
}

uint32_t RicohRP2C02::getRgb(const uint8_t tblIndex, const uint16_t pixelVal) const
{
    return colors[(localRead(PPU::PALETTE::Base + (tblIndex << 0x2) + pixelVal)) & 0x3F];
}

const uint32_t *RicohRP2C02::getFrameBuffData() const
{
    return frameBuffer.data();
}

uint8_t RicohRP2C02::localRead(uint16_t addr) const
{
    return bus->read(addr & 0x3FFF);
}

void RicohRP2C02::localWrite(uint16_t addr, uint8_t data)
{
    bus->write(addr & 0x3FFF, data);
}

void RicohRP2C02::run()
{
    if (scanline == -1 && cycles == 1)
    {
        statusRegister.vertical_blank = 0x0;
    }
	else if (scanline == 241 && cycles == 1)
	{
		statusRegister.vertical_blank = 0x1;

		if (controlRegister.enable_nmi) 
			requestCpuNmi = true;
	}

    ++cycles;

    if (cycles >= 341)
    {
        cycles = 0;
        ++scanline;

        if (scanline >= 261)
        {
            scanline = -1;
            // frameDrawn = true;
        }
    }
}