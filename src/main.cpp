#include <memory>
#include <cstdint>

#include <Ricoh2A03.hpp>
#include <RicohRP2C02.hpp>

int main(int argc, char *argv[])
{
    std::unique_ptr<Ricoh2A03> cpu;
    std::unique_ptr<RicohRP2C02> ppu;
    uint32_t sysClock = 0;
    ++sysClock;
    return 0;
}