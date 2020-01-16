#pragma once
#include <SDL2/SDL.h>
#include <MOS6502Instruction.hpp>
#include <Ricoh2A03.hpp>
#include <stdio.h>

template <Ricoh2A03::AddressingType T>
class AddressingMode : public MOS6502Instruction
{
    const std::string addrModeStr;

protected:
    AddressingMode(const std::string mnemonic, Ricoh2A03 *cpu, uint8_t numCycles);

    uint8_t fetchAuxData(bool loadData = true) override final;

    void writeBack() override final
    {
        if (T == Ricoh2A03::AddressingType::IMP)
            cpu->A = auxData;
        else
            cpu->write(absoluteAddress, auxData);
    }

    inline void instrDump()
    {
        printf("PC: 0x%04X / OP: %s / Addr: %s / Addr: 0x%04X / Data: 0x%02X\n",
               oldPC, mnemonic.c_str(), addrModeStr.c_str(), absoluteAddress, auxData);
        absoluteAddress = auxData = 0;
        printf("A: 0x%02X / X: 0x%02X / Y: 0x%02X / SP: 0x%02X / S: 0x%02X\n-----\n",
               cpu->A, cpu->X, cpu->Y, cpu->SP, cpu->S);
    }

public:
    std::string disassemble(uint16_t &pc) override;
};