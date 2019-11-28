#include <Ricoh2A03.hpp>
#include <Bus.hpp>
#include <Instructions.hpp>

#define GEN_INSTR(name, type, cycles) std::unique_ptr<MOS6502Instruction>(new name<AddressingType::type>(this, (cycles))

Ricoh2A03::Ricoh2A03(std::shared_ptr<Bus> bus) : bus{bus},
                                                 instructions{
                                                     GEN_INSTR(BRK, IMM, 7),
                                                     GEN_INSTR(ORA, INX, 6),
                                                     GEN_INSTR(NOP, IMP, 2),
                                                     GEN_INSTR(NOP, IMP, 8),
                                                     GEN_INSTR(NOP, IMP, 3),
                                                     GEN_INSTR(ORA, ZP, 3),
                                                     GEN_INSTR(ASL, ZP, 5),
                                                     GEN_INSTR(NOP, IMP, 5),
                                                     GEN_INSTR(PHP, IMP, 4),
                                                     GEN_INSTR(ORA, IMM, 2),
                                                     GEN_INSTR(ASL, IMP, 2),
                                                     GEN_INSTR(NOP, IMP, 2),
                                                     GEN_INSTR(NOP, IMP, 4),
                                                     GEN_INSTR(ORA, AB, 4),
                                                     GEN_INSTR(ASL, AB, 6),
                                                     GEN_INSTR(NOP, IMP, 6),

                                                     GEN_INSTR(BPL, , 0, 2),
                                                     GEN_INSTR(ORA, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(NOP, , 0, 2),
                                                     GEN_INSTR(NOP, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(NOP, , 0, 2),
                                                     GEN_INSTR(ORA, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(ASL, , 0, 2),
                                                     GEN_INSTR(NOP, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(CLC, , 0, 2),
                                                     GEN_INSTR(ORA, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(NOP, , 0, 2),
                                                     GEN_INSTR(NOP, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(NOP, , 0, 2),
                                                     GEN_INSTR(ORA, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(ASL, , 0, 2),
                                                     GEN_INSTR(NOP, <AddressingType::IMP>, 0, 2),

                                                     GEN_INSTR(JSR, , 0, 2),
                                                     GEN_INSTR(AND, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(NOP, , 0, 2),
                                                     GEN_INSTR(NOP, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(BIT, , 0, 2),
                                                     GEN_INSTR(AND, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(ROL, , 0, 2),
                                                     GEN_INSTR(NOP, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(PLP, , 0, 2),
                                                     GEN_INSTR(AND, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(ROL, , 0, 2),
                                                     GEN_INSTR(NOP, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(BIT, , 0, 2),
                                                     GEN_INSTR(AND, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(ROL, , 0, 2),
                                                     GEN_INSTR(NOP, <AddressingType::IMP>, 0, 2),

                                                     GEN_INSTR(BMI, , 0, 2),
                                                     GEN_INSTR(AND, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(NOP, , 0, 2),
                                                     GEN_INSTR(NOP, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(NOP, , 0, 2),
                                                     GEN_INSTR(AND, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(ROL, , 0, 2),
                                                     GEN_INSTR(NOP, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(SEC, , 0, 2),
                                                     GEN_INSTR(AND, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(NOP, , 0, 2),
                                                     GEN_INSTR(NOP, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(NOP, , 0, 2),
                                                     GEN_INSTR(AND, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(ROL, , 0, 2),
                                                     GEN_INSTR(NOP, <AddressingType::IMP>, 0, 2),

                                                     GEN_INSTR(RTI, , 0, 2),
                                                     GEN_INSTR(EOR, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(NOP, , 0, 2),
                                                     GEN_INSTR(NOP, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(NOP, , 0, 2),
                                                     GEN_INSTR(EOR, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(LSR, , 0, 2),
                                                     GEN_INSTR(NOP, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(PHA, , 0, 2),
                                                     GEN_INSTR(EOR, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(LSR, , 0, 2),
                                                     GEN_INSTR(NOP, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(JMP, , 0, 2),
                                                     GEN_INSTR(EOR, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(LSR, , 0, 2),
                                                     GEN_INSTR(NOP, <AddressingType::IMP>, 0, 2),

                                                     GEN_INSTR(BVC, , 0, 2),
                                                     GEN_INSTR(EOR, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(NOP, , 0, 2),
                                                     GEN_INSTR(NOP, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(NOP, , 0, 2),
                                                     GEN_INSTR(EOR, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(LSR, , 0, 2),
                                                     GEN_INSTR(NOP, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(CLI, , 0, 2),
                                                     GEN_INSTR(EOR, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(NOP, , 0, 2),
                                                     GEN_INSTR(NOP, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(NOP, , 0, 2),
                                                     GEN_INSTR(EOR, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(LSR, , 0, 2),
                                                     GEN_INSTR(NOP, <AddressingType::IMP>, 0, 2),

                                                     GEN_INSTR(RTS, , 0, 2),
                                                     GEN_INSTR(ADC, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(NOP, , 0, 2),
                                                     GEN_INSTR(NOP, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(NOP, , 0, 2),
                                                     GEN_INSTR(ADC, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(ROR, , 0, 2),
                                                     GEN_INSTR(NOP, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(PLA, , 0, 2),
                                                     GEN_INSTR(ADC, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(ROR, , 0, 2),
                                                     GEN_INSTR(NOP, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(JMP, , 0, 2),
                                                     GEN_INSTR(ADC, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(ROR, , 0, 2),
                                                     GEN_INSTR(NOP, <AddressingType::IMP>, 0, 2),

                                                     GEN_INSTR(BVS, , 0, 2),
                                                     GEN_INSTR(ADC, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(NOP, , 0, 2),
                                                     GEN_INSTR(NOP, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(NOP, , 0, 2),
                                                     GEN_INSTR(ADC, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(ROR, , 0, 2),
                                                     GEN_INSTR(NOP, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(SEI, , 0, 2),
                                                     GEN_INSTR(ADC, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(NOP, , 0, 2),
                                                     GEN_INSTR(NOP, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(NOP, , 0, 2),
                                                     GEN_INSTR(ADC, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(ROR, , 0, 2),
                                                     GEN_INSTR(NOP, <AddressingType::IMP>, 0, 2),

                                                     GEN_INSTR(NOP, , 0, 2),
                                                     GEN_INSTR(STA, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(NOP, , 0, 2),
                                                     GEN_INSTR(NOP, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(STY, , 0, 2),
                                                     GEN_INSTR(STA, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(STX, , 0, 2),
                                                     GEN_INSTR(NOP, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(DEY, , 0, 2),
                                                     GEN_INSTR(NOP, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(TXA, , 0, 2),
                                                     GEN_INSTR(NOP, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(STY, , 0, 2),
                                                     GEN_INSTR(STA, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(STX, , 0, 2),
                                                     GEN_INSTR(NOP, <AddressingType::IMP>, 0, 2),

                                                     GEN_INSTR(BCC, , 0, 2),
                                                     GEN_INSTR(STA, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(NOP, , 0, 2),
                                                     GEN_INSTR(NOP, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(STY, , 0, 2),
                                                     GEN_INSTR(STA, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(STX, , 0, 2),
                                                     GEN_INSTR(NOP, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(TYA, , 0, 2),
                                                     GEN_INSTR(STA, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(TXS, , 0, 2),
                                                     GEN_INSTR(NOP, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(NOP, , 0, 2),
                                                     GEN_INSTR(STA, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(NOP, , 0, 2),
                                                     GEN_INSTR(NOP, <AddressingType::IMP>, 0, 2),

                                                     GEN_INSTR(LDY, , 0, 2),
                                                     GEN_INSTR(LDA, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(LDX, , 0, 2),
                                                     GEN_INSTR(NOP, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(LDY, , 0, 2),
                                                     GEN_INSTR(LDA, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(LDX, , 0, 2),
                                                     GEN_INSTR(NOP, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(TAY, , 0, 2),
                                                     GEN_INSTR(LDA, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(TAX, , 0, 2),
                                                     GEN_INSTR(NOP, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(LDY, , 0, 2),
                                                     GEN_INSTR(LDA, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(LDX, , 0, 2),
                                                     GEN_INSTR(NOP, <AddressingType::IMP>, 0, 2),

                                                     GEN_INSTR(BCS, , 0, 2),
                                                     GEN_INSTR(LDA, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(NOP, , 0, 2),
                                                     GEN_INSTR(NOP, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(LDY, , 0, 2),
                                                     GEN_INSTR(LDA, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(LDX, , 0, 2),
                                                     GEN_INSTR(NOP, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(CLV, , 0, 2),
                                                     GEN_INSTR(LDA, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(TSX, , 0, 2),
                                                     GEN_INSTR(NOP, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(LDY, , 0, 2),
                                                     GEN_INSTR(LDA, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(LDX, , 0, 2),
                                                     GEN_INSTR(NOP, <AddressingType::IMP>, 0, 2),

                                                     GEN_INSTR(CPY, , 0, 2),
                                                     GEN_INSTR(CMP, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(NOP, , 0, 2),
                                                     GEN_INSTR(NOP, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(CPY, , 0, 2),
                                                     GEN_INSTR(CMP, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(DEC, , 0, 2),
                                                     GEN_INSTR(NOP, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(INY, , 0, 2),
                                                     GEN_INSTR(CMP, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(DEX, , 0, 2),
                                                     GEN_INSTR(NOP, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(CPY, , 0, 2),
                                                     GEN_INSTR(CMP, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(DEC, , 0, 2),
                                                     GEN_INSTR(NOP, <AddressingType::IMP>, 0, 2),

                                                     GEN_INSTR(BNE, , 0, 2),
                                                     GEN_INSTR(CMP, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(NOP, , 0, 2),
                                                     GEN_INSTR(NOP, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(NOP, , 0, 2),
                                                     GEN_INSTR(CMP, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(DEC, , 0, 2),
                                                     GEN_INSTR(NOP, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(CLD, , 0, 2),
                                                     GEN_INSTR(CMP, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(NOP, , 0, 2),
                                                     GEN_INSTR(NOP, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(NOP, , 0, 2),
                                                     GEN_INSTR(CMP, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(DEC, , 0, 2),
                                                     GEN_INSTR(NOP, <AddressingType::IMP>, 0, 2),

                                                     GEN_INSTR(CPX, , 0, 2),
                                                     GEN_INSTR(SBC, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(NOP, , 0, 2),
                                                     GEN_INSTR(NOP, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(CPX, , 0, 2),
                                                     GEN_INSTR(SBC, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(INC, , 0, 2),
                                                     GEN_INSTR(NOP, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(INX, , 0, 2),
                                                     GEN_INSTR(SBC, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(NOP, , 0, 2),
                                                     GEN_INSTR(NOP, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(CPX, , 0, 2),
                                                     GEN_INSTR(SBC, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(INC, , 0, 2),
                                                     GEN_INSTR(NOP, <AddressingType::IMP>, 0, 2),

                                                     GEN_INSTR(BEQ, , 0, 2),
                                                     GEN_INSTR(SBC, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(NOP, , 0, 2),
                                                     GEN_INSTR(NOP, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(NOP, , 0, 2),
                                                     GEN_INSTR(SBC, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(INC, , 0, 2),
                                                     GEN_INSTR(NOP, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(SED, , 0, 2),
                                                     GEN_INSTR(SBC, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(NOP, , 0, 2),
                                                     GEN_INSTR(NOP, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(NOP, , 0, 2),
                                                     GEN_INSTR(SBC, <AddressingType::IMP>, 0, 2),
                                                     GEN_INSTR(INC, , 0, 2),
                                                     GEN_INSTR(NOP, <AddressingType::IMP>, 0, 2),
                                                 }

{
}

uint8_t Ricoh2A03::read(uint16_t addr)
{
    return bus->read(addr);
}

void Ricoh2A03::write(uint16_t addr, uint8_t data)
{
    bus->write(addr, data);
}

void Ricoh2A03::pushWord(uint8_t word)
{
    write(STACK_BASE + SP--, word);
}

void Ricoh2A03::pushDoubleWord(uint16_t dWord)
{
    write(STACK_BASE + SP--, dWord >> 8);
    write(STACK_BASE + SP--, dWord & 0x00FF);
}

uint8_t Ricoh2A03::popWord()
{
    return read(STACK_BASE + SP++);
}

uint16_t Ricoh2A03::popDoubleWord()
{
    uint16_t lo = read(STACK_BASE + SP++);
    uint16_t hi = read(STACK_BASE + SP++);
    return (hi << 8) | lo;
}

void Ricoh2A03::tick()
{
    MOS6502Instruction *currentInstr = nullptr;
    if (cycles == 0)
    {
        (void)currentInstr;
        // opcode = read(PC++);
        // cycles = ;
    }
    --cycles;
}

void Ricoh2A03::reset()
{
    A = X = Y = 0;
    SP = 0xFD;
    S = U;

    PC = (static_cast<uint16_t>(read(0xFFFD)) << 8) |
         static_cast<uint16_t>(read(0xFFFC));

    cycles = 8;
}

void Ricoh2A03::nmi(uint16_t interruptAddr)
{
    setFlag(U, true);
    setFlag(I, true);
    setFlag(B, false);

    pushDoubleWord(PC);
    pushWord(S);

    PC = (static_cast<uint16_t>(read(interruptAddr + 0x1)) << 8) |
         static_cast<uint16_t>(read(interruptAddr));

    cycles = 8;
}

void Ricoh2A03::irq()
{
    if (!getFlag(I))
    {
        nmi(0xFFFE);
        --cycles;
    }
}

bool Ricoh2A03::getFlag(Flags6502 f) const
{
    return static_cast<bool>(S & f);
}

void Ricoh2A03::setFlag(Flags6502 f, bool b)
{
    S &= ~(1 << f);
    S |= (1 << static_cast<uint8_t>(b));
}

uint8_t Ricoh2A03::branch(uint8_t auxData, uint16_t absoluteAddress, bool cond)
{
    uint8_t cyclePenalty = 0;

    if (cond)
    {
        cyclePenalty = 1;
        absoluteAddress = PC + auxData;

        if ((PC & 0xFF00) != (absoluteAddress & 0xFF00))
            cyclePenalty = 2;
    }

    return cyclePenalty;
}