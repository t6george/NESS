#pragma once

class Bus
{
    unique_ptr<RamDevice> memory;
public:
    Bus();
    ~Bus();
    void write(uint16_t addr, uint8_t, data);
    uint8_t read(uint16_t addr, bool readOnly = false);
};