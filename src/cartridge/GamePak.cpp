#include <GamePak.hpp>

GamePak::GamePak(const std::string &fname, const uint16_t size, const uint16_t addrBase, const uint16_t addrEnd) : AddressableDevice::AddressableDevice(size, addrBase, addrEnd) {}

void GamePak::setByte(uint16_t addr, uint8_t data)
{
}

uint8_t GamePak::getByte(uint16_t addr, bool readOnly) const
{
    return 0;
}