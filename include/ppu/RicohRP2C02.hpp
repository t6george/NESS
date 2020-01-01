#pragma once
#include <memory>
#include <array>
#include <vector>

#include <AddressableDevice.hpp>
#include <HwConstants.hpp>
#include <SDL2/SDL.h>

class Bus;
class GamePak;
class NesSystem;

class RicohRP2C02 : public AddressableDevice
{
    uint16_t cycle;
    uint16_t addrLatch;
    uint8_t dataBuffer;
    int16_t scanline;
    bool requestCpuNmi;
    uint8_t fine_x;

    uint8_t bg_next_tile_id = 0x00;
    uint8_t bg_next_tile_attrib = 0x00;
    uint8_t bg_next_tile_lsb = 0x00;
    uint8_t bg_next_tile_msb = 0x00;
    uint16_t bg_shifter_pattern_lo = 0x0000;
    uint16_t bg_shifter_pattern_hi = 0x0000;
    uint16_t bg_shifter_attrib_lo = 0x0000;
    uint16_t bg_shifter_attrib_hi = 0x0000;
    uint16_t bgShiftL = 0, bgShiftH = 0;
    uint16_t dot = 0x0;
    bool oddFrame = false;
    bool updateScreen = false;
    uint8_t at, bgL, bgH, nt, atShiftH = 0x0, atShiftL = 0x0;
    bool atLatchL = false, atLatchH = false;

    std::unique_ptr<Bus> bus;
    std::shared_ptr<GamePak> cartridge;

    std::array<uint32_t, DISPLAY::ColorCount> colors;
    std::vector<uint32_t> frameBuffer;

    uint8_t localRead(uint16_t addr) const;
    void localWrite(uint16_t addr, uint8_t data);

    union {
        uint8_t raw;

        struct
        {
            uint8_t scratch : 5;
            uint8_t sprite_overflow : 1;
            uint8_t sprite_zero : 1;
            uint8_t vertical_blank : 1;
        };

    } statusRegister;

    union {
        uint8_t raw;

        struct
        {
            uint8_t grayscale : 1;
            uint8_t render_bg_left : 1;
            uint8_t render_sprites_left : 1;
            uint8_t render_bg : 1;
            uint8_t render_sprites : 1;
            uint8_t enhance_r : 1;
            uint8_t enhance_g : 1;
            uint8_t enhance_b : 1;
        };

    } maskRegister;

    union {
        uint8_t raw;

        struct
        {
            uint8_t nametable : 2;
            uint8_t inc_mode : 1;
            uint8_t pattern_sprite : 1;
            uint8_t pattern_bg : 1;
            uint8_t sprite_size : 1;
            uint8_t slave_mode : 1;
            uint8_t enable_nmi : 1;
        };

    } controlRegister;

    union LoopyRegister {
        uint16_t raw;

        struct
        {
            uint8_t coarse_x : 5;
            uint8_t coarse_y : 5;
            uint8_t nametable : 2;
            uint8_t fine_y : 3;
            uint8_t scratch : 1;
        };
    };

    LoopyRegister vramAddr;
    LoopyRegister tramAddr;

    bool rendering() { return maskRegister.render_bg || maskRegister.render_sprites; }

    void updatePixel(bool framePrep = false);

    uint16_t nt_addr() { return 0x2000 | (vramAddr.raw & 0xFFF); }
    uint16_t at_addr() { return 0x23C0 | (vramAddr.nametable << 10) | ((vramAddr.coarse_y / 4) << 3) | (vramAddr.coarse_x / 4); }
    uint16_t bg_addr() { return (controlRegister.pattern_bg * 0x1000) + (nt * 16) + vramAddr.fine_y; }

    void h_scroll()
    {
        if (!rendering())
            return;
        if (vramAddr.coarse_x == 31)
            vramAddr.raw ^= 0x41F;
        else
            vramAddr.coarse_x++;
    }
    void v_scroll()
    {
        if (!rendering())
            return;
        if (vramAddr.fine_y < 7)
            vramAddr.fine_y++;
        else
        {
            vramAddr.fine_y = 0;
            if (vramAddr.coarse_y == 31)
                vramAddr.coarse_y = 0;
            else if (vramAddr.coarse_y == 29)
            {
                vramAddr.coarse_y = 0;
                vramAddr.nametable ^= 0b10;
            }
            else
                vramAddr.coarse_y++;
        }
    }

    void reload_shift()
    {
        bgShiftL = (bgShiftL & 0xFF00) | bgL;
        bgShiftH = (bgShiftH & 0xFF00) | bgH;

        atLatchL = (at & 1);
        atLatchH = (at & 2);
    }

    void h_update()
    {
        if (!rendering())
            return;
        vramAddr.raw = (vramAddr.raw & ~0x041F) | (tramAddr.raw & 0x041F);
    }

    void v_update()
    {
        if (!rendering())
            return;
        vramAddr.raw = (vramAddr.raw & ~0x7BE0) | (tramAddr.raw & 0x7BE0);
    }

#define NTH_BIT(x, n) (((x) >> (n)) & 1)
    void pixel()
    {
        uint8_t palette = 0, objPalette = 0;
        bool objPriority = 0;
        int x = dot - 2;

        if (scanline < 240 and x >= 0 and x < 256)
        {
            if (maskRegister.render_bg and not(!maskRegister.render_bg_left && x < 8))
            {
                // SDL_Log("%x  %x  %x  %x", bgShiftH, bgShiftL, atShiftH, atShiftL);
                // Background:
                palette = (NTH_BIT(bgShiftH, 15 - fine_x) << 1) |
                          NTH_BIT(bgShiftL, 15 - fine_x);
                if (palette)
                    palette |= ((NTH_BIT(atShiftH, 7 - fine_x) << 1) |
                                NTH_BIT(atShiftL, 7 - fine_x))
                               << 2;
            }
            // Sprites:
            // if (mask.spr and not (!mask.sprLeft && x < 8))
            //     for (int i = 7; i >= 0; i--)
            //     {
            //         if (oam[i].id == 64) continue;  // Void entry.
            //         unsigned sprX = x - oam[i].x;
            //         if (sprX >= 8) continue;            // Not in range.
            //         if (oam[i].attr & 0x40) sprX ^= 7;  // Horizontal flip.
            //
            //         u8 sprPalette = (NTH_BIT(oam[i].dataH, 7 - sprX) << 1) |
            //                          NTH_BIT(oam[i].dataL, 7 - sprX);
            //         if (sprPalette == 0) continue;  // Transparent pixel.
            //
            //         if (oam[i].id == 0 && palette && x != 255) status.sprHit = true;
            //         sprPalette |= (oam[i].attr & 3) << 2;
            //         objPalette  = sprPalette + 16;
            //         objPriority = oam[i].attr & 0x20;
            //     }
            // Evaluate priority:
            if (objPalette && (palette == 0 || objPriority == 0))
                palette = objPalette;
            // if (palette != 0)
            //   SDL_Log("mux %x", 0x3F00 + (rendering() ? palette : 0));
            frameBuffer[scanline * 256 + x] = colors[localRead(0x3F00 + (rendering() ? palette : 0))];
        }
        // Perform background shifts:
        bgShiftL <<= 1;
        bgShiftH <<= 1;
        atShiftL = (atShiftL << 1) | atLatchL;
        atShiftH = (atShiftH << 1) | atLatchH;
    }

protected:
    void setByte(uint16_t addr, uint8_t data) override;
    uint8_t getByte(uint16_t addr, bool readOnly) override;

public:
    RicohRP2C02();
    void addCartridge(std::shared_ptr<AddressableDevice> cart);
    void run();

    void updateFrameBuffer(const uint8_t tblIndex);
    uint32_t getRgb(const uint8_t tblIndex, const uint16_t pixelVal) const;
    const uint32_t *getFrameBuffData() const;
    void reset();

    friend class NesSystem;
};