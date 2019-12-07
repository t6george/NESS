#pragma once
#include <cstdint>
#include <memory>
#include <string>

class AddressableDevice;

class NesSystem
{
    uint32_t clock;
    std::unique_ptr<AddressableDevice> ppu;
    std::unique_ptr<AddressableDevice> cpu;

public:
    NesSystem();
    ~NesSystem() = default;
    void tick();
    void reset();
    void insertCartridge(const std::string &romName);
};