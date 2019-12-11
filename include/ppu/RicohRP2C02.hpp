#pragma once
#include <memory>
#include <array>
#include <vector>

#include <AddressableDevice.hpp>
#include <HwConstants.hpp>

class Bus;
class GamePak;

class RicohRP2C02 : public AddressableDevice
{
    uint16_t cycles;

    std::unique_ptr<Bus> bus;
    std::shared_ptr<GamePak> cartridge;

    std::array<uint32_t, DISPLAY::ColorCount> colors;
    std::vector<uint32_t> frameBuffer;

protected:
    void setByte(uint16_t addr, uint8_t data) override;
    uint8_t getByte(uint16_t addr, bool readOnly) const override;

public:
    RicohRP2C02();
    void addCartridge(std::shared_ptr<AddressableDevice> cart);
    void run();

    void updateFrameBuffer(const uint8_t tblIndex);
    uint32_t getRgb(const uint8_t tblIndex, const uint16_t pixelVal) const;
    const uint32_t *getFrameBuffData() const;
};