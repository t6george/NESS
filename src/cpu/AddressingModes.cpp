#include <AddressingModes.hpp>
#include <Ricoh2A03.hpp>

/*
 * Instruction is acting on some register, and
 * does not need data from RAM
 */
template <>
uint8_t AddressingMode<Ricoh2A03::AddressingType::IMP>::fetchAuxData()
{
    auxData = cpu->A;
    return 0;
}

/*
 * Instruction has been given an immediate value,
 * so no need to access RAM either
 */
template <>
uint8_t AddressingMode<Ricoh2A03::AddressingType::IMM>::fetchAuxData()
{
    absoluteAddress = cpu->PC++;
    auxData = cpu->read(absoluteAddress);
    return 0;
}

/*
 * Instruction is given an 8-bit address.
 * The upper byte is implicitly 0x00, so
 * it accesses the RAM's Page 0
 */
template <>
uint8_t AddressingMode<Ricoh2A03::AddressingType::ZP>::fetchAuxData()
{
    absoluteAddress = static_cast<uint16_t>(cpu->read(cpu->PC++));
    auxData = cpu->readZ(absoluteAddress);
    return 0;
}

/*
 * Zero Page Addressing with an offset
 * equal to CPU REG X
 */
template <>
uint8_t AddressingMode<Ricoh2A03::AddressingType::ZPX>::fetchAuxData()
{
    absoluteAddress = static_cast<uint16_t>(cpu->read(cpu->PC++) + cpu->X);
    auxData = cpu->readZ(absoluteAddress);
    return 0;
}

/*
 * Zero Page Addressing with an offset
 * equal to CPU REG Y
 */
template <>
uint8_t AddressingMode<Ricoh2A03::AddressingType::ZPY>::fetchAuxData()
{
    absoluteAddress = static_cast<uint16_t>(cpu->read(cpu->PC++) + cpu->Y);
    auxData = cpu->readZ(absoluteAddress);
    return 0;
}

/*
 * Branch offset located at CPU's PC
 * Offset is only 8-bit, so must be sign-extended
 */
template <>
uint8_t AddressingMode<Ricoh2A03::AddressingType::REL>::fetchAuxData()
{
    absoluteAddress = cpu->PC++;
    absoluteAddress = cpu->read(absoluteAddress);
    absoluteAddress |= ((static_cast<uint8_t>(0x80 & auxData) != 0x0000) * 0xFF00);

    return 0;
}

/*
 * Given a direct 16-bit address
 * to retrieve the contents of
 */
template <>
uint8_t AddressingMode<Ricoh2A03::AddressingType::AB>::fetchAuxData()
{
    absoluteAddress = cpu->readDoubleWord(cpu->PC);
    cpu->PC += 0x2;

    auxData = cpu->read(absoluteAddress);
    return 0;
}

/*
 * Absolute addressing with an offset of CPU REG X
 * Page cross => 1 cycle penalty
 */
template <>
uint8_t AddressingMode<Ricoh2A03::AddressingType::ABX>::fetchAuxData()
{
    absoluteAddress = cpu->readDoubleWord(cpu->PC) + cpu->X;
    cpu->PC += 0x2;

    auxData = cpu->read(absoluteAddress);
    return (absoluteAddress & 0xFF00) != ((absoluteAddress - cpu->X) & 0xFF00);
}

/*
 * Absolute addressing with an offset of CPU REG Y
 * Page cross => 1 cycle penalty
 */
template <>
uint8_t AddressingMode<Ricoh2A03::AddressingType::ABY>::fetchAuxData()
{
    absoluteAddress = cpu->readDoubleWord(cpu->PC) + cpu->Y;
    cpu->PC += 0x2;

    auxData = cpu->read(absoluteAddress);
    return (absoluteAddress & 0xFF00) != ((absoluteAddress - cpu->Y) & 0xFF00);
}

/*
 * Address of pointer is encoded in instruction
 * instead of an address of data (like the above
 * addressing modes).
 */
template <>
uint8_t AddressingMode<Ricoh2A03::AddressingType::IN>::fetchAuxData()
{
    absoluteAddress = cpu->readDoubleWord(cpu->PC);
    cpu->PC += 0x2;

    // Hardware bug: wrap around if low addr byte's bits are all set (no carry is propagated)
    if ((absoluteAddress & 0x00FF) == 0x00FF)
    {
        absoluteAddress = ((static_cast<uint16_t>(cpu->read(absoluteAddress & 0xFF00)) << 8) |
                           static_cast<uint16_t>(cpu->read(absoluteAddress)));
    }
    else
    {
        absoluteAddress = cpu->readDoubleWord(absoluteAddress);
    }

    auxData = cpu->read(absoluteAddress);

    return 0;
}

/*
 * Indirect addressing, but address of pointer
 * is offset by CPU REG X
 */
template <>
uint8_t AddressingMode<Ricoh2A03::AddressingType::IX>::fetchAuxData()
{
    absoluteAddress = static_cast<uint16_t>(cpu->read(cpu->PC++)) + static_cast<uint16_t>(cpu->X);
    absoluteAddress = cpu->readZDoubleWord(absoluteAddress);

    auxData = cpu->read(absoluteAddress);

    return 0;
}

/*
 * Indirect addressing, but the actual pointer
 * in memory is offset by CPU REG Y right before
 * the data read.
 * Page cross => 1 cycle penalty
 */
template <>
uint8_t AddressingMode<Ricoh2A03::AddressingType::IY>::fetchAuxData()
{
    absoluteAddress = static_cast<uint16_t>(cpu->read(cpu->PC++));
    absoluteAddress = cpu->readZDoubleWord(absoluteAddress) + cpu->Y;

    auxData = cpu->read(absoluteAddress);

    return (absoluteAddress & 0xFF00) != ((absoluteAddress - cpu->Y) & 0xFF00);
}