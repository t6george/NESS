#pragma once
#include <cstdint>
#include <memory>
#include <vector>
#include <array>

#include <AddressableDevice.hpp>
#include <GamePak.hpp>

class RicohRP2C02 : public AddressableDevice
{
    uint8_t tblName[2][1024];
    uint8_t tblPalette[32];
    inline void scrollX();
    inline void scrollY();
    inline void tax();
    inline void tay();
    inline void loadShifters();
    inline void updateShifters();

    std::array<uint32_t, 0x40> palettes;
    std::vector<uint32_t> sprScreen;

    union {
        struct
        {
            uint8_t unused : 5;
            uint8_t sprite_overflow : 1;
            uint8_t sprite_zero_hit : 1;
            uint8_t vertical_blank : 1;
        };

        uint8_t reg;
    } status;

    union {
        struct
        {
            uint8_t grayscale : 1;
            uint8_t render_background_left : 1;
            uint8_t render_sprites_left : 1;
            uint8_t render_background : 1;
            uint8_t render_sprites : 1;
            uint8_t enhance_red : 1;
            uint8_t enhance_green : 1;
            uint8_t enhance_blue : 1;
        };

        uint8_t reg;
    } mask;

    union PPUCTRL {
        struct
        {
            uint8_t nametable_x : 1;
            uint8_t nametable_y : 1;
            uint8_t increment_mode : 1;
            uint8_t pattern_sprite : 1;
            uint8_t pattern_background : 1;
            uint8_t sprite_size : 1;
            uint8_t slave_mode : 1;
            uint8_t enable_nmi : 1;
        };

        uint8_t reg;
    } control;

    union loopy_register {
        struct
        {

            uint16_t coarse_x : 5;
            uint16_t coarse_y : 5;
            uint16_t nametable_x : 1;
            uint16_t nametable_y : 1;
            uint16_t fine_y : 3;
            uint16_t unused : 1;
        };

        uint16_t reg = 0x0000;
    };

    loopy_register vram_addr;
    loopy_register tram_addr;

    uint8_t fine_x = 0x00;

    uint8_t address_latch = 0x00;
    uint8_t ppu_data_buffer = 0x00;

    int16_t scanline = 0;
    int16_t cycle = 0;

    uint8_t bg_next_tile_id = 0x00;
    uint8_t bg_next_tile_attrib = 0x00;
    uint8_t bg_next_tile_lsb = 0x00;
    uint8_t bg_next_tile_msb = 0x00;
    uint16_t bg_shifter_pattern_lo = 0x0000;
    uint16_t bg_shifter_pattern_hi = 0x0000;
    uint16_t bg_shifter_attrib_lo = 0x0000;
    uint16_t bg_shifter_attrib_hi = 0x0000;
    GamePak *cart;

    struct sObjectAttributeEntry
    {
        uint8_t y;
        uint8_t id;
        uint8_t attribute;
        uint8_t x;
    } OAM[64];

    uint8_t oam_addr = 0x00;

    sObjectAttributeEntry spriteScanline[8];
    uint8_t sprite_count;
    uint8_t sprite_shifter_pattern_lo[8];
    uint8_t sprite_shifter_pattern_hi[8];

    bool bSpriteZeroHitPossible = false;
    bool bSpriteZeroBeingRendered = false;
    
public:
    RicohRP2C02();
    ~RicohRP2C02();
    uint32_t *getFrameBuffData();
    uint32_t GetColourFromPaletteRam(uint8_t palette, uint8_t pixel);
    uint8_t *pOAM = (uint8_t *)OAM;

    uint8_t getByte(uint16_t addr, bool readOnly = false) override;
    void setByte(uint16_t addr, uint8_t data) override;

    uint8_t localRead(uint16_t addr, bool rdonly = false);
    void localWrite(uint16_t addr, uint8_t data);

    void addCartridge(const std::shared_ptr<AddressableDevice> cartridge);
    void run();
    void reset();
    bool requestCpuNmi = false;
};
