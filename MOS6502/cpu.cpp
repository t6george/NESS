#include <iostream>

#include "../Headers/cpu.hpp"

using namespace std;

CPU::CPU () {}
CPU::~CPU () {}

CPU CPU::getInstance () {
  if (NULL == mos6502) {
    mos6502 = new CPU ();
  }
  return mos6502;
}

void CPU::processInstruction (int instr) {

}
