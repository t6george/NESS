#pragma once
#include <memory>

class Bus;

class Ricoh2A03
{
    shared_ptr<Bus> bus;

    public:
    Ricoh2A03();
    ~Ricoh2A03();
    uint8_t read(uint16_t addr);
    void write(uint16_t addr, uint8_t data);
};