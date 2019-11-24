#include <Instructions.hpp>

BRK::BRK(Ricoh2A03* cpu, uint8_t numCycles, uint8_t size): 
    MOS6502Instruction::MOS6502Instruction(cpu, numCycles, size) {}

BRK::exec()
{
    cpu->SetFlag(I, true);
    return numCycles;
}