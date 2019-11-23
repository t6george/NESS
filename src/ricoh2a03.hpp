#pragma once
#include <memory>

class Bus;

class Ricoh2A03
{
    shared_ptr<Bus> bus;
    public:

    enum Flags6502
    {
        C = (1 << 0);
        Z = (1 << 1);
        I = (1 << 2);
        D = (1 << 3);
        B = (1 << 4);
        U = (1 << 5);
        V = (1 << 6);
        N = (1 << 7);
    };

    uint8_t A;
    uint8_t X;
    uint8_t Y;
    uint8_t SP;
    uint16_t PC;
    uint8_t S;

    uint8_t fetch();
    uint8_t fetched;
    uint16_t abs_addr;
    uint8_t opcode;
    uint8_t cycles;
     

    Ricoh2A03(shared_ptr<Bus> bus);
    ~Ricoh2A03();
    uint8_t read(uint16_t addr);
    void write(uint16_t addr, uint8_t data);

    void clockTick();
    void reset();
    void irq();
    void nmi();

    bool GetFlag(Flags6502 f);
    void SetFlag(Flags6502 f, bool b)
};