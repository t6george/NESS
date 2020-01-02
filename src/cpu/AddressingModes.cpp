#include <sstream>
#include <iomanip>

#include <AddressingModes.hpp>
#include <Ricoh2A03.hpp>

/*
 * Instruction is acting on some register, and
 * does not need data from RAM
 */
template <>
AddressingMode<Ricoh2A03::AddressingType::IMP>::AddressingMode(const std::string mnemonic, Ricoh2A03 *cpu, uint8_t numCycles)
    : MOS6502Instruction(mnemonic, cpu, numCycles), addrModeStr{"IMP"} {}

template <>
std::string AddressingMode<Ricoh2A03::AddressingType::IMP>::disassemble(uint16_t &pc)
{
    (void)pc;

    return mnemonic;
}

template <>
uint8_t AddressingMode<Ricoh2A03::AddressingType::IMP>::fetchAuxData(bool loadData)
{
    oldPC = cpu->PC - 0x1;

    if (loadData)
        auxData = cpu->A;

    return 0;
}

/*
 * Instruction has been given an immediate value,
 * so no need to access RAM either
 */
template <>
AddressingMode<Ricoh2A03::AddressingType::IMM>::AddressingMode(const std::string mnemonic, Ricoh2A03 *cpu, uint8_t numCycles)
    : MOS6502Instruction(mnemonic, cpu, numCycles), addrModeStr{"IMM"} {}

template <>
std::string AddressingMode<Ricoh2A03::AddressingType::IMM>::disassemble(uint16_t &pc)
{
    std::ostringstream stream;
    stream << mnemonic << " $" << std::hex << std::uppercase << static_cast<int>(cpu->read(pc++));

    return stream.str();
}

template <>
uint8_t AddressingMode<Ricoh2A03::AddressingType::IMM>::fetchAuxData(bool loadData)
{
    oldPC = cpu->PC - 0x1;
    absoluteAddress = cpu->PC++;

    if (loadData)
        auxData = cpu->read(absoluteAddress);

    return 0;
}

/*
 * Instruction is given an 8-bit address.
 * The upper byte is implicitly 0x00, so
 * it accesses the RAM's Page 0
 */
template <>
AddressingMode<Ricoh2A03::AddressingType::ZP>::AddressingMode(const std::string mnemonic, Ricoh2A03 *cpu, uint8_t numCycles)
    : MOS6502Instruction(mnemonic, cpu, numCycles), addrModeStr{"ZPI"} {}

template <>
std::string AddressingMode<Ricoh2A03::AddressingType::ZP>::disassemble(uint16_t &pc)
{
    std::ostringstream stream;
    stream << mnemonic << " $" << std::internal << std::setfill('0') << std::setw(2) << std::hex << std::uppercase << static_cast<int>(cpu->read(pc++));

    return stream.str();
}

template <>
uint8_t AddressingMode<Ricoh2A03::AddressingType::ZP>::fetchAuxData(bool loadData)
{
    oldPC = cpu->PC - 0x1;
    absoluteAddress = static_cast<uint16_t>(cpu->read(cpu->PC++));

    if (loadData)
        auxData = cpu->read(absoluteAddress, true);

    return 0;
}

/*
 * Zero Page Addressing with an offset
 * equal to CPU REG X
 */
template <>
AddressingMode<Ricoh2A03::AddressingType::ZPX>::AddressingMode(const std::string mnemonic, Ricoh2A03 *cpu, uint8_t numCycles)
    : MOS6502Instruction(mnemonic, cpu, numCycles), addrModeStr{"ZPX"} {}

template <>
std::string AddressingMode<Ricoh2A03::AddressingType::ZPX>::disassemble(uint16_t &pc)
{
    std::ostringstream stream;
    stream << mnemonic << " $" << std::internal << std::setfill('0') << std::setw(2) << std::hex << std::uppercase << static_cast<int>(cpu->read(pc++)) << ", X";

    return stream.str();
}

template <>
uint8_t AddressingMode<Ricoh2A03::AddressingType::ZPX>::fetchAuxData(bool loadData)
{
    oldPC = cpu->PC - 0x1;
    absoluteAddress = static_cast<uint16_t>(cpu->read(cpu->PC++) + cpu->X);

    if (loadData)
        auxData = cpu->read(absoluteAddress, true);

    return 0;
}

/*
 * Zero Page Addressing with an offset
 * equal to CPU REG Y
 */
template <>
AddressingMode<Ricoh2A03::AddressingType::ZPY>::AddressingMode(const std::string mnemonic, Ricoh2A03 *cpu, uint8_t numCycles)
    : MOS6502Instruction(mnemonic, cpu, numCycles), addrModeStr{"ZPY"} {}

template <>
std::string AddressingMode<Ricoh2A03::AddressingType::ZPY>::disassemble(uint16_t &pc)
{
    std::ostringstream stream;
    stream << mnemonic << " $" << std::internal << std::setfill('0') << std::setw(2) << std::hex << std::uppercase << static_cast<int>(cpu->read(pc++)) << ", Y";

    return stream.str();
}

template <>
uint8_t AddressingMode<Ricoh2A03::AddressingType::ZPY>::fetchAuxData(bool loadData)
{
    oldPC = cpu->PC - 0x1;
    absoluteAddress = static_cast<uint16_t>(cpu->read(cpu->PC++) + cpu->Y);

    if (loadData)
        auxData = cpu->read(absoluteAddress, true);

    return 0;
}

/*
 * Branch offset located at CPU's PC
 * Offset is only 8-bit, so must be sign-extended
 */
template <>
AddressingMode<Ricoh2A03::AddressingType::REL>::AddressingMode(const std::string mnemonic, Ricoh2A03 *cpu, uint8_t numCycles)
    : MOS6502Instruction(mnemonic, cpu, numCycles), addrModeStr{"REL"} {}

template <>
std::string AddressingMode<Ricoh2A03::AddressingType::REL>::disassemble(uint16_t &pc)
{
    std::ostringstream stream;
    stream << mnemonic << " $" << std::internal << std::setfill('0') << std::setw(2) << std::hex << std::uppercase << static_cast<int>(cpu->read(pc++));

    return stream.str();
}

template <>
uint8_t AddressingMode<Ricoh2A03::AddressingType::REL>::fetchAuxData(bool loadData)
{
    oldPC = cpu->PC - 0x1;
    absoluteAddress = cpu->read(cpu->PC++);
    absoluteAddress |= ((static_cast<uint8_t>(0x80 & absoluteAddress) != 0x0000) * 0xFF00);

    return 0;
}

/*
 * Given a direct 16-bit address
 * to retrieve the contents of
 */
template <>
AddressingMode<Ricoh2A03::AddressingType::AB>::AddressingMode(const std::string mnemonic, Ricoh2A03 *cpu, uint8_t numCycles)
    : MOS6502Instruction(mnemonic, cpu, numCycles), addrModeStr{"ABS"} {}

template <>
std::string AddressingMode<Ricoh2A03::AddressingType::AB>::disassemble(uint16_t &pc)
{
    std::ostringstream stream;
    stream << mnemonic << " $" << std::internal << std::setfill('0') << std::setw(4) << std::hex << std::uppercase << static_cast<int>(cpu->readDoubleWord(pc));
    pc += 0x2;

    return stream.str();
}

template <>
uint8_t AddressingMode<Ricoh2A03::AddressingType::AB>::fetchAuxData(bool loadData)
{
    oldPC = cpu->PC - 0x1;
    absoluteAddress = cpu->readDoubleWord(cpu->PC);
    cpu->PC += 0x2;

    if (loadData)
        auxData = cpu->read(absoluteAddress);

    return 0;
}

/*
 * Absolute addressing with an offset of CPU REG X
 * Page cross => 1 cycle penalty
 */
template <>
AddressingMode<Ricoh2A03::AddressingType::ABX>::AddressingMode(const std::string mnemonic, Ricoh2A03 *cpu, uint8_t numCycles)
    : MOS6502Instruction(mnemonic, cpu, numCycles), addrModeStr{"ABX"} {}

template <>
std::string AddressingMode<Ricoh2A03::AddressingType::ABX>::disassemble(uint16_t &pc)
{
    std::ostringstream stream;
    stream << mnemonic << " $" << std::internal << std::setfill('0') << std::setw(4) << std::hex << std::uppercase << static_cast<int>(cpu->readDoubleWord(pc)) << ", X";
    pc += 0x2;

    return stream.str();
}

template <>
uint8_t AddressingMode<Ricoh2A03::AddressingType::ABX>::fetchAuxData(bool loadData)
{
    oldPC = cpu->PC - 0x1;
    absoluteAddress = cpu->readDoubleWord(cpu->PC) + cpu->X;
    cpu->PC += 0x2;

    if (loadData)
        auxData = cpu->read(absoluteAddress);

    return (absoluteAddress & 0xFF00) != ((absoluteAddress - cpu->X) & 0xFF00);
}

/*
 * Absolute addressing with an offset of CPU REG Y
 * Page cross => 1 cycle penalty
 */
template <>
AddressingMode<Ricoh2A03::AddressingType::ABY>::AddressingMode(const std::string mnemonic, Ricoh2A03 *cpu, uint8_t numCycles)
    : MOS6502Instruction(mnemonic, cpu, numCycles), addrModeStr{"ABY"} {}

template <>
std::string AddressingMode<Ricoh2A03::AddressingType::ABY>::disassemble(uint16_t &pc)
{
    std::ostringstream stream;
    stream << mnemonic << " $" << std::internal << std::setfill('0') << std::setw(4) << std::hex << std::uppercase << static_cast<int>(cpu->readDoubleWord(pc)) << ", Y";
    pc += 0x2;

    return stream.str();
}

template <>
uint8_t AddressingMode<Ricoh2A03::AddressingType::ABY>::fetchAuxData(bool loadData)
{
    oldPC = cpu->PC - 0x1;
    absoluteAddress = cpu->readDoubleWord(cpu->PC) + cpu->Y;
    cpu->PC += 0x2;

    if (loadData)
        auxData = cpu->read(absoluteAddress);

    return (absoluteAddress & 0xFF00) != ((absoluteAddress - cpu->Y) & 0xFF00);
}

/*
 * Address of pointer is encoded in instruction
 * instead of an address of data (like the above
 * addressing modes).
 */
template <>
AddressingMode<Ricoh2A03::AddressingType::IN>::AddressingMode(const std::string mnemonic, Ricoh2A03 *cpu, uint8_t numCycles)
    : MOS6502Instruction(mnemonic, cpu, numCycles), addrModeStr{"IND"} {}

template <>
std::string AddressingMode<Ricoh2A03::AddressingType::IN>::disassemble(uint16_t &pc)
{
    std::ostringstream stream;
    stream << mnemonic << " ($" << std::internal << std::setfill('0') << std::setw(4) << std::hex << std::uppercase << static_cast<int>(cpu->readDoubleWord(pc)) << ")";
    pc += 0x2;

    return stream.str();
}

template <>
uint8_t AddressingMode<Ricoh2A03::AddressingType::IN>::fetchAuxData(bool loadData)
{
    oldPC = cpu->PC - 0x1;
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

    if (loadData)
        auxData = cpu->read(absoluteAddress);

    return 0;
}

/*
 * Indirect addressing, but address of pointer
 * is offset by CPU REG X
 */
template <>
AddressingMode<Ricoh2A03::AddressingType::IX>::AddressingMode(const std::string mnemonic, Ricoh2A03 *cpu, uint8_t numCycles)
    : MOS6502Instruction(mnemonic, cpu, numCycles), addrModeStr{"INX"} {}

template <>
std::string AddressingMode<Ricoh2A03::AddressingType::IX>::disassemble(uint16_t &pc)
{
    std::ostringstream stream;
    stream << mnemonic << " ($" << std::internal << std::setfill('0') << std::setw(2) << std::hex << std::uppercase << static_cast<int>(cpu->readDoubleWord(pc)) << ", X)";
    pc += 0x2;

    return stream.str();
}

template <>
uint8_t AddressingMode<Ricoh2A03::AddressingType::IX>::fetchAuxData(bool loadData)
{
    oldPC = cpu->PC - 0x1;
    absoluteAddress = static_cast<uint16_t>(cpu->read(cpu->PC++)) + static_cast<uint16_t>(cpu->X);
    absoluteAddress = cpu->readDoubleWord(absoluteAddress, true);

    if (loadData)
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
AddressingMode<Ricoh2A03::AddressingType::IY>::AddressingMode(const std::string mnemonic, Ricoh2A03 *cpu, uint8_t numCycles)
    : MOS6502Instruction(mnemonic, cpu, numCycles), addrModeStr{"INY"} {}

template <>
std::string AddressingMode<Ricoh2A03::AddressingType::IY>::disassemble(uint16_t &pc)
{
    std::ostringstream stream;
    stream << mnemonic << " ($" << std::internal << std::setfill('0') << std::setw(2) << std::hex << std::uppercase << static_cast<int>(cpu->readDoubleWord(pc)) << "), Y";
    pc += 0x2;
    return stream.str();
}

template <>
uint8_t AddressingMode<Ricoh2A03::AddressingType::IY>::fetchAuxData(bool loadData)
{
    oldPC = cpu->PC - 0x1;
    absoluteAddress = static_cast<uint16_t>(cpu->read(cpu->PC++));
    absoluteAddress = cpu->readDoubleWord(absoluteAddress, true) + cpu->Y;

    if (loadData)
        auxData = cpu->read(absoluteAddress);

    return (absoluteAddress & 0xFF00) != ((absoluteAddress - cpu->Y) & 0xFF00);
}