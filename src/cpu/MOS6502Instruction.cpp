#include <MOS6502Instruction.hpp>
#include <Ricoh2A03.hpp>

MOS6502Instruction::MOS6502Instruction(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size) : cpu{cpu}, auxData{0x0000}, numCycles{numCycles}, size{size} {}