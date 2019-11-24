#pragma once
#include <cstdint>
#include <AddressingModes.hpp>

class Ricoh2A03;

class BRK : public Addressing::Implied
{
public:
    BRK(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size);
    uint8_t exec() override final;
};