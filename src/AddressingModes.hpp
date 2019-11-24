#pragma once
#include <MOS6502Instruction.hpp>
namespace Addressing
{
    class Implied: public MOS6502Instruction
    {
        public:
        uint8_t fetchAuxData() override final;
    };

    class Immmediate: public MOS6502Instruction
    {
        public:
        uint8_t fetchAuxData() override final;
    };

    class ZPage: public MOS6502Instruction
    {
        public:
        uint8_t fetchAuxData() override final;
    };

    class ZXPage: public MOS6502Instruction
    {
        public:
        uint8_t fetchAuxData() override final;
    };

    class ZYPage: public MOS6502Instruction
    {
        public:
        uint8_t fetchAuxData() override final;
    };

    class Relative: public MOS6502Instruction
    {
        public:
        uint8_t fetchAuxData() override final;
    };

    class Absolute: public MOS6502Instruction
    {
        public:
        uint8_t fetchAuxData() override final;
    };

    class AbsoluteX: public MOS6502Instruction
    {
        public:
        uint8_t fetchAuxData() override final;
    };

    class AbsoluteY: public MOS6502Instruction
    {
        public:
        uint8_t fetchAuxData() override final;
    };

    class Indirect: public MOS6502Instruction
    {
        public:
        uint8_t fetchAuxData() override final;
    };

    class IndirectZX: public MOS6502Instruction
    {
        public:
        uint8_t fetchAuxData() override final;
    };

    class IndirectZY: public MOS6502Instruction
    {
        public:
        uint8_t fetchAuxData() override final;
    };
}