#pragma once
#include <cstdint>
#include <memory>
#include <string>

class Ricoh2A03;
class RicohRP2C02;

class NesSystem
{
    uint32_t clock;
    std::shared_ptr<RicohRP2C02> ppu;
    std::unique_ptr<Ricoh2A03> cpu;

public:
    NesSystem();
    ~NesSystem() = default;
    void tick();
    void reset();
    void insertCartridge(const std::string &romName);
};