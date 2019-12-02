#pragma once
#include <Bus.hpp>

class CpuBus : public Bus
{
    enum Devices
    {
        RAM = 0,
        PPU,
        CART,
        TOTAL_DEVICES
    };

protected:
    virtual void initMmu() override;
    virtual std::shared_ptr<AddressableDevice> delegate(uint16_t& addr) override;
};