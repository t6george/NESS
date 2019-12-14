#pragma once
#include <memory>
#include <array>
#include <vector>

#include <AddressableDevice.hpp>
#include <HwConstants.hpp>

class Bus;
class GamePak;
class NesSystem;

class RicohRP2C02 : public AddressableDevice
{
    uint16_t cycles;
    uint16_t addrLatch;
    uint16_t ppuAddr;
    uint8_t dataBuffer;
    int16_t scanline;
    bool requestCpuNmi;
    // bool frameDrawn;

    std::unique_ptr<Bus> bus;
    std::shared_ptr<GamePak> cartridge;

    std::array<uint32_t, DISPLAY::ColorCount> colors;
    std::vector<uint32_t> frameBuffer;

    uint8_t localRead(uint16_t addr) const;
    void localWrite(uint16_t addr, uint8_t data);

    union
    {
        uint8_t raw;

        struct
        {
            uint8_t scratch            : 5;
            uint8_t sprite_overflow    : 1;
            uint8_t sprite_zero        : 1;
            uint8_t vertical_blank     : 1;
        };

    } statusRegister;
    
    union
    {
        uint8_t raw;

        struct
        {
            uint8_t grayscale              : 1;
            uint8_t render_bg_left         : 1;
            uint8_t render_sprites_left    : 1;
            uint8_t render_bg              : 1;
            uint8_t render_sprites         : 1;
            uint8_t enhance_r              : 1;
            uint8_t enhance_g              : 1;
            uint8_t enhance_b              : 1;
        };

    } maskRegister;

    union
    {
        uint8_t raw;

        struct
        {
            uint8_t nametable_x       : 1;
            uint8_t nametable_y       : 1;
            uint8_t inc_mode          : 1;
            uint8_t pattern_sprite    : 1;
            uint8_t pattern_bg        : 1;
            uint8_t sprite_size       : 1;
            uint8_t slave_mode        : 1;
            uint8_t enable_nmi        : 1;
        };

    } controlRegister;
    
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

    friend class NesSystem;
};