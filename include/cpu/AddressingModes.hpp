#pragma once
#include <MOS6502Instruction.hpp>
#include <Ricoh2A03.hpp>

template <Ricoh2A03::AddressingType T>
class AddressingMode : public MOS6502Instruction
{
protected:
    AddressingMode(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size) : MOS6502Instruction(cpu, numCycles, size){};

    uint8_t fetchAuxData() override final;

    void writeBack() override final
    {
        cpu->write(absoluteAddress, auxData);
    }
};