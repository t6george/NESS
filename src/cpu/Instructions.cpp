#include <Instructions.hpp>
#include <Ricoh2A03.hpp>

BRK::BRK(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size) : Addressing::Implied(cpu, numCycles, size) {}

uint8_t BRK::exec()
{
    cpu->SetFlag(Ricoh2A03::I, true);
    return numCycles;
}