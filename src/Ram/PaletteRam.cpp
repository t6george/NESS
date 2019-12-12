#include <PaletteRam.hpp>

PaletteRam::PaletteRam(const uint16_t size) : Ram::Ram{size} {}
    
uint8_t PaletteRam::read(uint16_t addr, uint16_t mirror, bool readOnly = false)
{
    
}

void PaletteRam::write(uint16_t addr, uint16_t mirror, uint8_t data)
{

}