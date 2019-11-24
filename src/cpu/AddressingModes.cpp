#include <AddressingModes.hpp>
#include <Ricoh2A03.hpp>

Addressing::Implied::Implied(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size) : MOS6502Instruction(cpu, numCycles, size) {}

uint8_t Addressing::Implied::fetchAuxData()
{
    auxData = cpu->A;
    return 0;
}

Addressing::Immediate::Immediate(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size) : MOS6502Instruction(cpu, numCycles, size) {}

uint8_t Addressing::Immediate::fetchAuxData()
{
    auxData = cpu->PC++;
    return 0;
}

Addressing::ZPage::ZPage(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size) : MOS6502Instruction(cpu, numCycles, size) {}

uint8_t Addressing::ZPage::fetchAuxData()
{
    auxData = static_cast<uint16_t>(cpu->read(cpu->PC++));
    return 0;
}

Addressing::ZXPage::ZXPage(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size) : MOS6502Instruction(cpu, numCycles, size) {}

uint8_t Addressing::ZXPage::fetchAuxData()
{
    auxData = static_cast<uint16_t>(cpu->read(cpu->PC++)) + cpu->X;
    return 0;
}

Addressing::ZYPage::ZYPage(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size) : MOS6502Instruction(cpu, numCycles, size) {}

uint8_t Addressing::ZYPage::fetchAuxData()
{
    auxData = static_cast<uint16_t>(cpu->read(cpu->PC++)) + cpu->Y;
    return 0;
}

Addressing::Relative::Relative(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size) : MOS6502Instruction(cpu, numCycles, size) {}

uint8_t Addressing::Relative::fetchAuxData()
{
    auxData = static_cast<uint16_t>(cpu->read(cpu->PC++));
    auxData |= (((0x80 & auxData) != 0x0000) * 0xFF00);
    return 0;
}

Addressing::Absolute::Absolute(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size) : MOS6502Instruction(cpu, numCycles, size) {}

uint8_t Addressing::Absolute::fetchAuxData()
{
    auxData = (static_cast<uint16_t>(cpu->read(cpu->PC + 0x1)) << 8) |
              static_cast<uint16_t>(cpu->read(cpu->PC));
    cpu->PC += 0x2;
    return 0;
}

Addressing::AbsoluteX::AbsoluteX(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size) : MOS6502Instruction(cpu, numCycles, size) {}

uint8_t Addressing::AbsoluteX::fetchAuxData()
{
    auxData = ((static_cast<uint16_t>(cpu->read(cpu->PC + 0x1)) << 8) |
               static_cast<uint16_t>(cpu->read(cpu->PC))) +
              cpu->X;
    cpu->PC += 0x2;
    return (auxData & 0xFF00) != ((auxData - cpu->X) & 0xFF00);
}

Addressing::AbsoluteY::AbsoluteY(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size) : MOS6502Instruction(cpu, numCycles, size) {}

uint8_t Addressing::AbsoluteY::fetchAuxData()
{
    auxData = ((static_cast<uint16_t>(cpu->read(cpu->PC + 0x1)) << 8) |
               static_cast<uint16_t>(cpu->read(cpu->PC))) +
              cpu->Y;
    cpu->PC += 0x2;
    return (auxData & 0xFF00) != ((auxData - cpu->Y) & 0xFF00);
}

Addressing::Indirect::Indirect(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size) : MOS6502Instruction(cpu, numCycles, size) {}

uint8_t Addressing::Indirect::fetchAuxData()
{
    auxData = ((static_cast<uint16_t>(cpu->read(cpu->PC + 0x1)) << 8) |
               static_cast<uint16_t>(cpu->read(cpu->PC)));
    cpu->PC += 0x2;

    // Hardware bug: wrap around if low addr byte's bits are all set (no carry is propagated)
    if ((auxData & 0x00FF) == 0x00FF)
    {
        auxData = ((static_cast<uint16_t>(cpu->read(auxData & 0xFF00)) << 8) |
                   static_cast<uint16_t>(cpu->read(auxData)));
    }
    else
    {
        auxData = ((static_cast<uint16_t>(cpu->read(auxData + 0x1)) << 8) |
                   static_cast<uint16_t>(cpu->read(auxData)));
    }

    return 0;
}

Addressing::IndirectZX::IndirectZX(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size) : MOS6502Instruction(cpu, numCycles, size) {}

uint8_t Addressing::IndirectZX::fetchAuxData()
{
    auxData = static_cast<uint16_t>(cpu->read(cpu->PC++) + cpu->X);
    auxData = ((static_cast<uint16_t>(cpu->read(auxData + 0x1)) << 8) |
               static_cast<uint16_t>(cpu->read(auxData)));

    return 0;
}

Addressing::IndirectZY::IndirectZY(Ricoh2A03 *cpu, uint8_t numCycles, uint8_t size) : MOS6502Instruction(cpu, numCycles, size) {}

uint8_t Addressing::IndirectZY::fetchAuxData()
{
    auxData = static_cast<uint16_t>(cpu->read(cpu->PC++));
    auxData = ((static_cast<uint16_t>(cpu->read(auxData + 0x1)) << 8) |
               static_cast<uint16_t>(cpu->read(auxData))) +
              cpu->Y;

    return (auxData & 0xFF00) != ((auxData - cpu->Y) & 0xFF00);
}