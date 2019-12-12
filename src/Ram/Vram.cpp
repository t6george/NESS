#include <Vram.hpp>

VRam::VRam(const uint16_t size) : Ram::Ram{size} {}

virtual uint8_t VRam::read(uint16_t addr, uint16_t mirror, bool readOnly)
{
    
}

virtual void VRam::write(uint16_t addr, uint16_t mirror, uint8_t data)
{

}