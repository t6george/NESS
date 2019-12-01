#pragma once
#include <AddressableDevice.hpp>

class Bus;
class GamePak;

class RicohRP2C02 : public AddressableDevice
{
    std::unique_ptr<Bus> bus;
    std::shared_ptr<GamePak> cartridge;
public:
    RicohRP2C02();
    uint8_t read(uint16_t addr, bool readOnly = false) const override;
    void write(uint16_t addr, uint8_t data) override;
};