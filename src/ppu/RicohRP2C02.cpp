#include <cassert>

#include <RicohRP2C02.hpp>
#include <Bus.hpp>
#include <Vram.hpp>
#include <PaletteRam.hpp>
#include <HwConstants.hpp>
#include <GamePak.hpp>
#include <SDL2/SDL.h>

RicohRP2C02::RicohRP2C02() : cycle{0}, addrLatch{0x00}, dataBuffer{0x00},
                             scanline{0}, requestCpuNmi{false}, fine_x{0x00}, bus{new Bus{}},
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
#include <cassert>

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
        statusRegister.vertical_blank = 0x0;
        addrLatch = 0x0;
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
        dataBuffer = localRead(vramAddr.raw);

        if (vramAddr.raw >= PPU::PALETTE::Base && vramAddr.raw <= PPU::PALETTE::Limit)
        {
            data = dataBuffer;
        }

        vramAddr.raw += (controlRegister.inc_mode ? 0x20 : 0x1);
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
        tramAddr.nametable = controlRegister.nametable;
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
        if (addrLatch == 0x0)
        {
            fine_x = data & 0x07;
            tramAddr.coarse_x = data >> 0x3;
        }
        else
        {
            tramAddr.fine_y = data & 0x07;
            tramAddr.coarse_y = data >> 3;
        }
        addrLatch ^= 0x1;
        break;
    case 0x0006:
        if (addrLatch == 0x00)
        {
            tramAddr.raw = static_cast<uint16_t>((data & 0x3F) << 0x8) | (tramAddr.raw & 0x00FF);
        }
        else
        {
            tramAddr.raw = (tramAddr.raw & 0xFF00) | static_cast<uint16_t>(data);
            vramAddr = tramAddr;
        }

        addrLatch ^= 0x01;
        break;
    case 0x0007:
        localWrite(vramAddr.raw, data);
        vramAddr.raw += (controlRegister.inc_mode ? 0x20 : 0x1);
        break;
    default:
        break;
    }
}

void RicohRP2C02::addCartridge(std::shared_ptr<AddressableDevice> cart)
{
    GamePak *game = nullptr;
    assert(game = dynamic_cast<GamePak *>(cart.get()));

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
                frameBuffer[offset + row * DISPLAY::Width + col] =
                    getRgb(tblIndex, pixel);
            }
        }
    }
}

uint32_t RicohRP2C02::getRgb(const uint8_t tblIndex, const uint16_t pixelVal) const
{
    // int palette = PPU::PALETTE::Base + (tblIndex << 0x2) + pixelVal;
    // if (palette != PPU::PALETTE::Base)
    // colors[(localRead(PPU::PALETTE::Base + (tblIndex << 0x2) + pixelVal)) & maskRegister.grayscale ? 0x30 : 0x3F] | DISPLAY::PixelOpacity);
    return colors[(localRead(PPU::PALETTE::Base + (tblIndex << 0x2) + pixelVal)) & maskRegister.grayscale ? 0x30 : 0x3F] | DISPLAY::PixelOpacity;
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
void RicohRP2C02::updatePixel(bool framePrep)
{
    static uint16_t addr;
    // SDL_Log("status %x", statusRegister.raw);
    // SDL_Log("mask %x", maskRegister.raw);
    // SDL_Log("control %x", controlRegister.raw);
    // SDL_Log("vram %x", vramAddr.raw);
    // SDL_Log("tram %x", tramAddr.raw);
    // SDL_Log("-------");
    switch (dot)
    {
    case 2 ... 255:
    case 322 ... 337:
        pixel();
        switch (dot % 8)
        {
        // Nametable:
        case 1:
            addr = nt_addr();
            reload_shift();
            break;
        case 2:
            nt = localRead(addr);
            break;
        // Attribute:
        case 3:
            addr = at_addr();
            break;
        case 4:
            at = localRead(addr);
            if (vramAddr.coarse_y & 2)
                at >>= 4;
            if (vramAddr.coarse_x & 2)
                at >>= 2;
            break;
        // Background (low bits):
        case 5:
            addr = bg_addr();
            break;
        case 6:
            bgL = localRead(addr);
            break;
        // Background (high bits):
        case 7:
            addr += 8;
            break;
        case 0:
            bgH = localRead(addr);
            h_scroll();
            break;
        }
        break;
    case 256:
        pixel();
        bgH = localRead(addr);
        v_scroll();
        break; // Vertical bump.
    case 257:
        pixel();
        reload_shift();
        h_update();
        break; // Update horizontal position.
    case 280 ... 304:
        if (framePrep)
            v_update();
        break; // Update vertical position.

    // No shift reloading:
    case 1:
        addr = nt_addr();
        if (framePrep)
            statusRegister.vertical_blank = 0x0;
        break;
    case 321:
    case 339:
        addr = nt_addr();
        break;
    // Nametable fetch instead of attribute:
    case 338:
        nt = localRead(addr);
        break;
    case 340:
        nt = localRead(addr);
        if (framePrep && rendering() && oddFrame)
            ++dot;
    }
}
// static uint32_t cnt = 0;
// #define NTH_BIT(x, n) (((x) >> (n)) & 1)
#include <iostream>
void RicohRP2C02::run()
{
    // std::cout << "moss " << std::hex << " dot is " << static_cast<int>(dot) << "    " << static_cast<int>(cnt) << std::endl;
    // ++cnt;
    switch (scanline)
    {
    case 0 ... 239:
        updatePixel();
        break;
    case 240:
        if (dot == 0)
        {
            updateScreen = true;
        }
        break;
    case 241:
        if (dot == 0x1)
        {
            std::cout << "NMI" << std::endl;
            statusRegister.vertical_blank = 0x1;
            // std::cout << "ctrl reg " << std::hex << static_cast<int>(controlRegister.raw) << std::endl;
            if (controlRegister.enable_nmi == 0x1)
            {
                requestCpuNmi = true;
            }
        }
        break;
    case 261:
        updatePixel(true);
        break;
    }

    if (++dot > 340)
    {
        dot %= 341;
        if (++scanline > 261)
        {
            scanline = 0;
            oddFrame ^= true;
        }
    }
}

void RicohRP2C02::reset()
{
    std::cout << "RESET" << std::endl;
    fine_x = 0x00;
    addrLatch = 0x00;
    dataBuffer = 0x00;
    scanline = 0;
    cycle = 0;
    bg_next_tile_id = 0x00;
    bg_next_tile_attrib = 0x00;
    bg_next_tile_lsb = 0x00;
    bg_next_tile_msb = 0x00;
    bg_shifter_pattern_lo = 0x0000;
    bg_shifter_pattern_hi = 0x0000;
    bg_shifter_attrib_lo = 0x0000;
    bg_shifter_attrib_hi = 0x0000;
    statusRegister.raw = 0x00;
    maskRegister.raw = 0x00;
    controlRegister.raw = 0x00;
    vramAddr.raw = 0x0000;
    tramAddr.raw = 0x0000;
    dot = 0;
}