#include <DualAddressableDevice.hpp>

DualAddressableDevice::DualAddressableDevice(const std::vector<uint16_t> &sizes, const uint16_t primBase,
                                             const uint16_t primEnd, const uint16_t seconBase, const uint16_t seconEnd);

bool DualAddressableDevice::writeTo(uint16_t addr, uint8_t data)
{
}

uint8_t DualAddressableDevice::readFrom(uint16_t addr, bool readOnly = false) const
{
}

void DualAddressableDevice::setMemorySignalRequester(Requester device)
{
}