#pragma once
#include <Bus.hpp>

class PpuBus : public Bus
{
    enum Devices
    {
        CART = 0,
        TOTAL_DEVICES
    };

protected:
    virtual void initMmu() override;
    virtual std::shared_ptr<AddressableDevice> delegate(uint16_t& addr) override;
};