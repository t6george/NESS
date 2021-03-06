#include <cassert>

#include <Ricoh2A03.hpp>
#include <Ram.hpp>
#include <RicohRP2C02.hpp>
#include <Instructions.hpp>
#include <HwConstants.hpp>
#include <GamePak.hpp>
#include <GamePad.hpp>
#include <Apu2A03.hpp>

// Nested template syntax can be damaging to the eye - I only want to write it out once :)
#define GEN_INSTR(name, type, cycles) std::unique_ptr<MOS6502Instruction>(new name<Ricoh2A03::AddressingType::type>(this, cycles))

inline uint16_t Ricoh2A03::elapsed() const
{
    return FRAME_TICKS - remaining;
}

uint8_t Ricoh2A03::read(uint16_t addr, bool zpageMode)
{
    uint16_t mask = 0xFFFF;

    if (zpageMode)
        mask = 0x00FF;

    addr &= mask;

    if ((0x4000 <= addr && addr <= 0x4013) || addr == 0x4015)
    {
        return apu->access<0>(elapsed(), addr, 0);
    }

    return bus->read(addr & mask);
}

uint16_t Ricoh2A03::readDoubleWord(uint16_t addr, bool zpageMode)
{
    return (static_cast<uint16_t>(read(addr + 0x1, zpageMode)) << 8) |
           (static_cast<uint16_t>(read(addr, zpageMode)));
}

void Ricoh2A03::write(uint16_t addr, uint8_t data)
{
    if (addr == 0x4014)
    {
        dma_transfer = true;
        dma_page = data;
        dma_addr = 0x00;
    }
    else if (addr == 0x4017)
    {
        apu->access<1>(elapsed(), addr, data);
    }
    else if ((0x4000 <= addr && addr <= 0x4013) || addr == 0x4015)
    {
        apu->access<1>(elapsed(), addr, data);
    }
    else
    {
        bus->write(addr, data);
    }
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
    return read(STACK_BASE + ++SP);
}

uint16_t Ricoh2A03::popDoubleWord()
{
    uint16_t lo = read(STACK_BASE + ++SP);
    uint16_t hi = read(STACK_BASE + ++SP);
    return (hi << 8) | lo;
}

// Fetch-Execute Cycle
void Ricoh2A03::fetch()
{
    if (cycles == 0)
    {
        setFlag(U, true);
        uint8_t opcode = read(PC++);
        cycles = instructions[opcode]->exec();
        setFlag(U, true);
    }
    --cycles;
}

void Ricoh2A03::reset()
{
    A = X = Y = remaining = 0x00;
    SP = 0xFD;
    S = U;

    PC = readDoubleWord(0xFFFC);

    cycles = 0x8;
}

void Ricoh2A03::nmi(uint16_t interruptAddr)
{
    // std::cout << "NMI" << std::endl;
    setFlag(U, true);
    setFlag(I, true);
    setFlag(B, false);

    pushDoubleWord(PC);
    pushWord(S);

    PC = readDoubleWord(interruptAddr);

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
    S &= ~f;
    S |= (f * static_cast<uint8_t>(b));
}

uint8_t Ricoh2A03::branch(uint16_t absoluteAddress, bool cond)
{
    uint8_t cyclePenalty = 0;

    if (cond)
    {
        cyclePenalty = 1;
        PC += static_cast<int16_t>(absoluteAddress);

        if ((PC & 0xFF00) !=
            ((PC - static_cast<int16_t>(absoluteAddress)) & 0xFF00))
            cyclePenalty = 2;
    }

    return cyclePenalty;
}

void Ricoh2A03::addCartridge(std::shared_ptr<AddressableDevice> cart)
{
    assert(dynamic_cast<GamePak *>(cart.get()));

    cartridge = cart;

    bus->attachDevice(CPU::CARTRIDGE::Base,
                      CPU::CARTRIDGE::Limit,
                      CPU::CARTRIDGE::Mirror,
                      cart);
}

Ricoh2A03::Ricoh2A03(std::shared_ptr<AddressableDevice> ppu, std::shared_ptr<GamePad> p1)
    : bus{new Bus{p1}},
      instructions{
          GEN_INSTR(BRK, IMM, 7), GEN_INSTR(ORA, IX, 6),
          GEN_INSTR(NOP, IMP, 2), GEN_INSTR(NOP, IMP, 8),
          GEN_INSTR(NOP, IMP, 3), GEN_INSTR(ORA, ZP, 3),
          GEN_INSTR(ASL, ZP, 5), GEN_INSTR(NOP, IMP, 5),
          GEN_INSTR(PHP, IMP, 3), GEN_INSTR(ORA, IMM, 2),
          GEN_INSTR(ASL, IMP, 2), GEN_INSTR(NOP, IMP, 2),
          GEN_INSTR(NOP, IMP, 4), GEN_INSTR(ORA, AB, 4),
          GEN_INSTR(ASL, AB, 6), GEN_INSTR(NOP, IMP, 6),

          GEN_INSTR(BPL, REL, 2), GEN_INSTR(ORA, IY, 5),
          GEN_INSTR(NOP, IMP, 2), GEN_INSTR(NOP, IMP, 8),
          GEN_INSTR(NOP, IMP, 4), GEN_INSTR(ORA, ZPX, 4),
          GEN_INSTR(ASL, ZPX, 6), GEN_INSTR(NOP, IMP, 6),
          GEN_INSTR(CLC, IMP, 2), GEN_INSTR(ORA, ABY, 4),
          GEN_INSTR(NOP, IMP, 2), GEN_INSTR(NOP, IMP, 7),
          GEN_INSTR(NOP, IMP, 4), GEN_INSTR(ORA, ABX, 4),
          GEN_INSTR(ASL, ABX, 7), GEN_INSTR(NOP, IMP, 7),

          GEN_INSTR(JSR, AB, 6), GEN_INSTR(AND, IX, 6),
          GEN_INSTR(NOP, IMP, 2), GEN_INSTR(NOP, IMP, 8),
          GEN_INSTR(BIT, ZP, 3), GEN_INSTR(AND, ZP, 3),
          GEN_INSTR(ROL, ZP, 5), GEN_INSTR(NOP, IMP, 5),
          GEN_INSTR(PLP, IMP, 4), GEN_INSTR(AND, IMM, 2),
          GEN_INSTR(ROL, IMP, 2), GEN_INSTR(NOP, IMP, 2),
          GEN_INSTR(BIT, AB, 4), GEN_INSTR(AND, AB, 4),
          GEN_INSTR(ROL, AB, 6), GEN_INSTR(NOP, IMP, 6),

          GEN_INSTR(BMI, REL, 2), GEN_INSTR(AND, IY, 5),
          GEN_INSTR(NOP, IMP, 2), GEN_INSTR(NOP, IMP, 8),
          GEN_INSTR(NOP, IMP, 4), GEN_INSTR(AND, ZPX, 4),
          GEN_INSTR(ROL, ZPX, 6), GEN_INSTR(NOP, IMP, 6),
          GEN_INSTR(SEC, IMP, 2), GEN_INSTR(AND, ABY, 4),
          GEN_INSTR(NOP, IMP, 2), GEN_INSTR(NOP, IMP, 7),
          GEN_INSTR(NOP, IMP, 4), GEN_INSTR(AND, ABX, 4),
          GEN_INSTR(ROL, ABX, 7), GEN_INSTR(NOP, IMP, 7),

          GEN_INSTR(RTI, IMP, 6), GEN_INSTR(EOR, IX, 6),
          GEN_INSTR(NOP, IMP, 2), GEN_INSTR(NOP, IMP, 8),
          GEN_INSTR(NOP, IMP, 3), GEN_INSTR(EOR, ZP, 3),
          GEN_INSTR(LSR, ZP, 5), GEN_INSTR(NOP, IMP, 5),
          GEN_INSTR(PHA, IMP, 3), GEN_INSTR(EOR, IMM, 2),
          GEN_INSTR(LSR, IMP, 2), GEN_INSTR(NOP, IMP, 2),
          GEN_INSTR(JMP, AB, 3), GEN_INSTR(EOR, AB, 4),
          GEN_INSTR(LSR, AB, 6), GEN_INSTR(NOP, IMP, 6),

          GEN_INSTR(BVC, REL, 2), GEN_INSTR(EOR, IY, 5),
          GEN_INSTR(NOP, IMP, 2), GEN_INSTR(NOP, IMP, 8),
          GEN_INSTR(NOP, IMP, 4), GEN_INSTR(EOR, ZPX, 4),
          GEN_INSTR(LSR, ZPX, 6), GEN_INSTR(NOP, IMP, 6),
          GEN_INSTR(CLI, IMP, 2), GEN_INSTR(EOR, ABY, 4),
          GEN_INSTR(NOP, IMP, 2), GEN_INSTR(NOP, IMP, 7),
          GEN_INSTR(NOP, IMP, 4), GEN_INSTR(EOR, ABX, 4),
          GEN_INSTR(LSR, ABX, 7), GEN_INSTR(NOP, IMP, 7),

          GEN_INSTR(RTS, IMP, 6), GEN_INSTR(ADC, IX, 6),
          GEN_INSTR(NOP, IMP, 2), GEN_INSTR(NOP, IMP, 8),
          GEN_INSTR(NOP, IMP, 3), GEN_INSTR(ADC, ZP, 3),
          GEN_INSTR(ROR, ZP, 5), GEN_INSTR(NOP, IMP, 5),
          GEN_INSTR(PLA, IMP, 4), GEN_INSTR(ADC, IMM, 2),
          GEN_INSTR(ROR, IMP, 2), GEN_INSTR(NOP, IMP, 2),
          GEN_INSTR(JMP, IN, 5), GEN_INSTR(ADC, AB, 4),
          GEN_INSTR(ROR, AB, 6), GEN_INSTR(NOP, IMP, 6),

          GEN_INSTR(BVS, REL, 2), GEN_INSTR(ADC, IY, 5),
          GEN_INSTR(NOP, IMP, 2), GEN_INSTR(NOP, IMP, 8),
          GEN_INSTR(NOP, IMP, 4), GEN_INSTR(ADC, ZPX, 4),
          GEN_INSTR(ROR, ZPX, 6), GEN_INSTR(NOP, IMP, 6),
          GEN_INSTR(SEI, IMP, 2), GEN_INSTR(ADC, ABY, 4),
          GEN_INSTR(NOP, IMP, 2), GEN_INSTR(NOP, IMP, 7),
          GEN_INSTR(NOP, IMP, 4), GEN_INSTR(ADC, ABX, 4),
          GEN_INSTR(ROR, ABX, 7), GEN_INSTR(NOP, IMP, 7),

          GEN_INSTR(NOP, IMP, 2), GEN_INSTR(STA, IX, 6),
          GEN_INSTR(NOP, IMP, 2), GEN_INSTR(NOP, IMP, 6),
          GEN_INSTR(STY, ZP, 3), GEN_INSTR(STA, ZP, 3),
          GEN_INSTR(STX, ZP, 3), GEN_INSTR(NOP, IMP, 3),
          GEN_INSTR(DEY, IMP, 2), GEN_INSTR(NOP, IMP, 2),
          GEN_INSTR(TXA, IMP, 2), GEN_INSTR(NOP, IMP, 2),
          GEN_INSTR(STY, AB, 4), GEN_INSTR(STA, AB, 4),
          GEN_INSTR(STX, AB, 4), GEN_INSTR(NOP, IMP, 4),

          GEN_INSTR(BCC, REL, 2), GEN_INSTR(STA, IY, 6),
          GEN_INSTR(NOP, IMP, 2), GEN_INSTR(NOP, IMP, 6),
          GEN_INSTR(STY, ZPX, 4), GEN_INSTR(STA, ZPX, 4),
          GEN_INSTR(STX, ZPY, 4), GEN_INSTR(NOP, IMP, 4),
          GEN_INSTR(TYA, IMP, 2), GEN_INSTR(STA, ABY, 5),
          GEN_INSTR(TXS, IMP, 2), GEN_INSTR(NOP, IMP, 5),
          GEN_INSTR(NOP, IMP, 5), GEN_INSTR(STA, ABX, 5),
          GEN_INSTR(NOP, IMP, 5), GEN_INSTR(NOP, IMP, 5),

          GEN_INSTR(LDY, IMM, 2), GEN_INSTR(LDA, IX, 6),
          GEN_INSTR(LDX, IMM, 2), GEN_INSTR(NOP, IMP, 6),
          GEN_INSTR(LDY, ZP, 3), GEN_INSTR(LDA, ZP, 3),
          GEN_INSTR(LDX, ZP, 3), GEN_INSTR(NOP, IMP, 3),
          GEN_INSTR(TAY, IMP, 2), GEN_INSTR(LDA, IMM, 2),
          GEN_INSTR(TAX, IMP, 2), GEN_INSTR(NOP, IMP, 2),
          GEN_INSTR(LDY, AB, 4), GEN_INSTR(LDA, AB, 4),
          GEN_INSTR(LDX, AB, 4), GEN_INSTR(NOP, IMP, 4),

          GEN_INSTR(BCS, REL, 2), GEN_INSTR(LDA, IY, 5),
          GEN_INSTR(NOP, IMP, 2), GEN_INSTR(NOP, IMP, 5),
          GEN_INSTR(LDY, ZPX, 4), GEN_INSTR(LDA, ZPX, 4),
          GEN_INSTR(LDX, ZPY, 4), GEN_INSTR(NOP, IMP, 4),
          GEN_INSTR(CLV, IMP, 2), GEN_INSTR(LDA, ABY, 4),
          GEN_INSTR(TSX, IMP, 2), GEN_INSTR(NOP, IMP, 4),
          GEN_INSTR(LDY, ABX, 4), GEN_INSTR(LDA, ABX, 4),
          GEN_INSTR(LDX, ABY, 4), GEN_INSTR(NOP, IMP, 4),

          GEN_INSTR(CPY, IMM, 2), GEN_INSTR(CMP, IX, 6),
          GEN_INSTR(NOP, IMP, 2), GEN_INSTR(NOP, IMP, 8),
          GEN_INSTR(CPY, ZP, 3), GEN_INSTR(CMP, ZP, 3),
          GEN_INSTR(DEC, ZP, 5), GEN_INSTR(NOP, IMP, 5),
          GEN_INSTR(INY, IMP, 2), GEN_INSTR(CMP, IMM, 2),
          GEN_INSTR(DEX, IMP, 2), GEN_INSTR(NOP, IMP, 2),
          GEN_INSTR(CPY, AB, 4), GEN_INSTR(CMP, AB, 4),
          GEN_INSTR(DEC, AB, 6), GEN_INSTR(NOP, IMP, 6),

          GEN_INSTR(BNE, REL, 2), GEN_INSTR(CMP, IY, 5),
          GEN_INSTR(NOP, IMP, 2), GEN_INSTR(NOP, IMP, 8),
          GEN_INSTR(NOP, IMP, 4), GEN_INSTR(CMP, ZPX, 4),
          GEN_INSTR(DEC, ZPX, 6), GEN_INSTR(NOP, IMP, 6),
          GEN_INSTR(CLD, IMP, 2), GEN_INSTR(CMP, ABY, 4),
          GEN_INSTR(NOP, IMP, 2), GEN_INSTR(NOP, IMP, 7),
          GEN_INSTR(NOP, IMP, 4), GEN_INSTR(CMP, ABX, 4),
          GEN_INSTR(DEC, ABX, 7), GEN_INSTR(NOP, IMP, 7),

          GEN_INSTR(CPX, IMM, 2), GEN_INSTR(SBC, IX, 6),
          GEN_INSTR(NOP, IMP, 2), GEN_INSTR(NOP, IMP, 8),
          GEN_INSTR(CPX, ZP, 3), GEN_INSTR(SBC, ZP, 3),
          GEN_INSTR(INC, ZP, 5), GEN_INSTR(NOP, IMP, 5),
          GEN_INSTR(INX, IMP, 2), GEN_INSTR(SBC, IMM, 2),
          GEN_INSTR(NOP, IMP, 2), GEN_INSTR(SBC, IMP, 2),
          GEN_INSTR(CPX, AB, 4), GEN_INSTR(SBC, AB, 4),
          GEN_INSTR(INC, AB, 6), GEN_INSTR(NOP, IMP, 6),

          GEN_INSTR(BEQ, REL, 2), GEN_INSTR(SBC, IY, 5),
          GEN_INSTR(NOP, IMP, 2), GEN_INSTR(NOP, IMP, 8),
          GEN_INSTR(NOP, IMP, 4), GEN_INSTR(SBC, ZPX, 4),
          GEN_INSTR(INC, ZPX, 6), GEN_INSTR(NOP, IMP, 6),
          GEN_INSTR(SED, IMP, 2), GEN_INSTR(SBC, ABY, 4),
          GEN_INSTR(NOP, IMP, 2), GEN_INSTR(NOP, IMP, 7),
          GEN_INSTR(NOP, IMP, 4), GEN_INSTR(SBC, ABX, 4),
          GEN_INSTR(INC, ABX, 7), GEN_INSTR(INC, IMP, 7)},
      dma_page{0x00}, dma_addr{0x00}, dma_transfer{false},
      apu{new Apu2A03()}
{
    ram.reset(new Ram(CPU::RAM::Size));
    Ppu = ppu;
    bus->attachDevice(CPU::RAM::Base,
                      CPU::RAM::Limit,
                      CPU::RAM::Mirror,
                      ram);

    bus->attachDevice(CPU::PPU::Base,
                      CPU::PPU::Limit,
                      CPU::PPU::Mirror,
                      ppu);
}

void Ricoh2A03::processFrameAudio() const
{
    apu->run_frame(elapsed());
}

bool Ricoh2A03::isFrameDone() const
{
    return remaining <= 0;
}

void Ricoh2A03::restartFrameTimer()
{
    remaining += FRAME_TICKS;
}