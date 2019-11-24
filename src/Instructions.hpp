#pragma once
#include <AddressingModes.hpp>

class BRK : public Addressing::Implied
{
    public:
    MOS6502Instruction(Ricoh2A03* cpu, uint8_t numCycles, uint8_t size);
    uint8_t exec() override final;
};