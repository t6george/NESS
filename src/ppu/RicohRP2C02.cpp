#include <cassert>

#include <RicohRP2C02.hpp>
#include <Bus.hpp>
#include <Vram.hpp>
#include <PaletteRam.hpp>
#include <HwConstants.hpp>
#include <GamePak.hpp>

RicohRP2C02::RicohRP2C02() : cycle{0}, addrLatch{0x1}, dataBuffer{0x00}, 
      scanline{0}, requestCpuNmi{false}, fine_x{0x00}, bus{new Bus{}},
      colors{0x666666, 0x002A88, 0x1412A7, 0x3B00A4,
             0x5C007E, 0x6E0040, 0x6C0600, 0x561D00,
             0x333500, 0x0B4800, 0x005200, 0x004F08,
             0x00404D, 0x000000, 0x000000, 0x000000,
             0xADADAD, 0x155FD9, 0x4240FF, 0x7527FE,
             0xA01ACC, 0xB71E7B, 0xB53120, 0x994E00,
             0x6B6D00, 0x388700, 0x0C9300, 0x008F32,
             0x007C8D, 0x000000, 0x000000, 0x000000,
             0xFFFEFF, 0x64B0FF, 0x9290FF, 0xC676FF,
             0xF36AFF, 0xFE6ECC, 0xFE8170, 0xEA9E22,
             0xBCBE00, 0x88D800, 0x5CE430, 0x45E082,
             0x48CDDE, 0x4F4F4F, 0x000000, 0x000000,
             0xFFFEFF, 0xC0DFFF, 0xD3D2FF, 0xE8C8FF,
             0xFBC2FF, 0xFEC4EA, 0xFECCC5, 0xF7D8A5,
             0xE4E594, 0xCFEF96, 0xBDF4AB, 0xB3F3CC,
             0xB5EBF2, 0xB8B8B8, 0x000000, 0x000000},
             frameBuffer{std::vector<uint32_t>(
                 DISPLAY::Width * DISPLAY::Height, 
                 DISPLAY::PixelOpacity)} 
{
    bus->attachDevice(PPU::PALETTE::Base,
                      PPU::PALETTE::Limit,
                      PPU::PALETTE::Mirror,
                      std::shared_ptr<AddressableDevice>(
                          new PaletteRam(PPU::PALETTE::Size)));
}

uint8_t RicohRP2C02::getByte(uint16_t addr, bool readOnly)
{
    uint8_t data = 0x00;

    switch (addr)
    {
    case 0x0000:
        break;
    case 0x0001:
        break;
    case 0x0002:
        statusRegister.scratch = dataBuffer & 0x1F;
        data = statusRegister.raw;
        statusRegister.vertical_blank = 0;
        addrLatch = 0x1;
        break;
    case 0x0003:
        break;
    case 0x0004:
        break;
    case 0x0005:
        break;
    case 0x0006:
        break;
    case 0x0007:
        data = dataBuffer;
        dataBuffer = localRead(vramAddr.raw);

        if(vramAddr.raw >= PPU::PALETTE::Base && vramAddr.raw <= PPU::PALETTE::Limit)
        {
            data = dataBuffer;
        }

        vramAddr.raw += (controlRegister.inc_mode ? 0x20 : 0x1);
        break;
    default:
        break;
    }

    return data;
}

void RicohRP2C02::setByte(uint16_t addr, uint8_t data)
{
    switch (addr)
    {
    case 0x0000:
        controlRegister.raw = data;
        tramAddr.nametable_x = controlRegister.nametable_x;
		tramAddr.nametable_y = controlRegister.nametable_y;
        break;
    case 0x0001:
        maskRegister.raw = data;
        break;
    case 0x0002:
        break;
    case 0x0003:
        break;
    case 0x0004:
        break;
    case 0x0005:
    	if (addrLatch == 0x1)
		{
			fine_x = data & 0x07;
			tramAddr.coarse_x = data >> 0x3;
		}
		else
		{
			tramAddr.fine_y = data & 0x07;
			tramAddr.coarse_y = data >> 3;
		}
        addrLatch ^= 0x1;
		break;
    case 0x0006:
        tramAddr.raw = (tramAddr.raw & ~(static_cast<uint16_t>(0xFF) << (addrLatch * 0x8))) 
            | (static_cast<uint16_t>(data) << (addrLatch * 0x8));

        if (addrLatch == 0x00)
            vramAddr = tramAddr;
        addrLatch ^= 0x01;
        break;
    case 0x0007:
        localWrite(vramAddr.raw, data);
        vramAddr.raw += (controlRegister.inc_mode ? 0x20 : 0x1);
        break;
    default:
        break;
    }
}

void RicohRP2C02::addCartridge(std::shared_ptr<AddressableDevice> cart)
{
    GamePak* game = nullptr;
    assert(game = dynamic_cast<GamePak*>(cart.get()));

    bus->attachDevice(PPU::CARTRIDGE::Base,
                      PPU::CARTRIDGE::Limit,
                      PPU::CARTRIDGE::Mirror,
                      cart);

    bus->attachDevice(PPU::VRAM::Base,
                      PPU::VRAM::Limit,
                      PPU::VRAM::Mirror,
                      std::shared_ptr<AddressableDevice>(
                          new VRam(PPU::VRAM::Size, game->getMirrorMode())));
}

void RicohRP2C02::updateFrameBuffer(const uint8_t tblIndex)
{
    uint16_t chrBase = tblIndex * PPU::CARTRIDGE::CHR::PartitionSize, pixel;
    uint8_t lsBits, msBits;

    for (uint16_t offset = 0; offset < PPU::CARTRIDGE::CHR::NumTiles; offset += 2)
    {
        for (uint16_t row = 0; row < 0x8; ++row)
        {
            lsBits = localRead(chrBase + row);
            msBits = localRead(chrBase + row + 0x8);

            for (uint16_t col = 0; col < 0x8; ++col)
            {
                pixel = ((lsBits & 0x80) >> 0x7) | ((msBits & 0x80) >> 0x6);
                lsBits <<= 0x1;
                msBits <<= 0x1;
                frameBuffer[offset + row * DISPLAY::Width + col] |= 
                    getRgb(tblIndex, pixel);
            }
        }
    }
}

uint32_t RicohRP2C02::getRgb(const uint8_t tblIndex, const uint16_t pixelVal) const
{
    return colors[(localRead(PPU::PALETTE::Base + (tblIndex << 0x2) + pixelVal)) 
        & maskRegister.grayscale ? 0x30 : 0x3F];
}

const uint32_t *RicohRP2C02::getFrameBuffData() const
{
    return frameBuffer.data();
}

uint8_t RicohRP2C02::localRead(uint16_t addr) const
{
    return bus->read(addr & 0x3FFF);
}

void RicohRP2C02::localWrite(uint16_t addr, uint8_t data)
{
    bus->write(addr & 0x3FFF, data);
}

void RicohRP2C02::run()
{
    auto IncrementScrollX = [&]()
	{
		if (maskRegister.render_bg || maskRegister.render_sprites)
		{
			if (vramAddr.coarse_x == 31)
			{
				vramAddr.coarse_x = 0;
				vramAddr.nametable_x = ~vramAddr.nametable_x;
			}
			else
			{
				++vramAddr.coarse_x;
			}
		}
	};


    auto IncrementScrollY = [&]()
	{
		if (maskRegister.render_bg || maskRegister.render_sprites)
		{
			if (vramAddr.fine_y < 7)
			{
				++vramAddr.fine_y;
			}
			else
			{
				vramAddr.fine_y = 0;

				if (vramAddr.coarse_y == 29)
				{
					vramAddr.coarse_y = 0;
					vramAddr.nametable_y = ~vramAddr.nametable_y;
				}
				else
				{
					++vramAddr.coarse_y;
				}
			}
		}
	};


    auto TransferAddressX = [&]()
	{
		if (maskRegister.render_bg || maskRegister.render_sprites)
		{
			vramAddr.nametable_x = tramAddr.nametable_x;
			vramAddr.coarse_x    = tramAddr.coarse_x;
		}
	};


	auto TransferAddressY = [&]()
	{
		if (maskRegister.render_bg || maskRegister.render_sprites)
		{
			vramAddr.fine_y      = tramAddr.fine_y;
			vramAddr.nametable_y = tramAddr.nametable_y;
			vramAddr.coarse_y    = tramAddr.coarse_y;
		}
	};

    auto LoadBackgroundShifters = [&]()
	{	
		bg_shifter_pattern_lo = (bg_shifter_pattern_lo & 0xFF00) | bg_next_tile_lsb;
		bg_shifter_pattern_hi = (bg_shifter_pattern_hi & 0xFF00) | bg_next_tile_msb;

		bg_shifter_attrib_lo  = (bg_shifter_attrib_lo & 0xFF00) | ((bg_next_tile_attrib & 0b01) ? 0xFF : 0x00);
		bg_shifter_attrib_hi  = (bg_shifter_attrib_hi & 0xFF00) | ((bg_next_tile_attrib & 0b10) ? 0xFF : 0x00);
	};

    auto UpdateShifters = [&]()
	{
		if (maskRegister.render_bg)
		{
			bg_shifter_pattern_lo <<= 1;
			bg_shifter_pattern_hi <<= 1;

			bg_shifter_attrib_lo <<= 1;
			bg_shifter_attrib_hi <<= 1;
		}
	};

    if (scanline >= -1 && scanline < 240)
	{		
		if (scanline == 0 && cycle == 0)
		{
			cycle = 1;
		}

		if (scanline == -1 && cycle == 1)
		{
			statusRegister.vertical_blank = 0;
		}


		if ((cycle >= 2 && cycle < 258) || (cycle >= 321 && cycle < 338))
		{
			UpdateShifters();
			
			switch ((cycle - 1) % 8)
			{
			case 0:
				LoadBackgroundShifters();

				bg_next_tile_id = localRead(0x2000 | (vramAddr.raw & 0x0FFF));
				break;
			case 2:
				bg_next_tile_attrib = localRead(0x23C0 | (vramAddr.nametable_y << 11) 
					                                 | (vramAddr.nametable_x << 10) 
					                                 | ((vramAddr.coarse_y >> 2) << 3) 
					                                 | (vramAddr.coarse_x >> 2));
								
				if (vramAddr.coarse_y & 0x02) bg_next_tile_attrib >>= 4;
				if (vramAddr.coarse_x & 0x02) bg_next_tile_attrib >>= 2;
				bg_next_tile_attrib &= 0x03;
				break;

			case 4: 
				bg_next_tile_lsb = localRead((controlRegister.pattern_bg << 12) 
					                       + ((uint16_t)bg_next_tile_id << 4) 
					                       + (vramAddr.fine_y) + 0);

				break;
			case 6:
				bg_next_tile_msb = localRead((controlRegister.pattern_bg << 12)
					                       + ((uint16_t)bg_next_tile_id << 4)
					                       + (vramAddr.fine_y) + 8);
				break;
			case 7:
				IncrementScrollX();
				break;
			}
		}

		if (cycle == 256)
		{
			IncrementScrollY();
		}

		if (cycle == 257)
		{
			LoadBackgroundShifters();
			TransferAddressX();
		}

		if (cycle == 338 || cycle == 340)
		{
			bg_next_tile_id = localRead(0x2000 | (vramAddr.raw & 0x0FFF));
		}

		if (scanline == -1 && cycle >= 280 && cycle < 305)
		{
			TransferAddressY();
		}
	}

	if (scanline >= 241 && scanline < 261)
	{
		if (scanline == 241 && cycle == 1)
		{
			statusRegister.vertical_blank = 1;

			if (controlRegister.enable_nmi) 
				requestCpuNmi = true;
		}
	}

	uint8_t bg_pixel = 0x00;
	uint8_t bg_palette = 0x00;

	if (maskRegister.render_bg)
	{
		uint16_t bit_mux = 0x8000 >> fine_x;

		uint8_t p0_pixel = (bg_shifter_pattern_lo & bit_mux) > 0;
		uint8_t p1_pixel = (bg_shifter_pattern_hi & bit_mux) > 0;

		bg_pixel = (p1_pixel << 1) | p0_pixel;

		uint8_t bg_pal0 = (bg_shifter_attrib_lo & bit_mux) > 0;
		uint8_t bg_pal1 = (bg_shifter_attrib_hi & bit_mux) > 0;
		bg_palette = (bg_pal1 << 1) | bg_pal0;
	}


    frameBuffer[scanline * 256 + cycle - 1] = getRgb(bg_palette, bg_pixel);

    ++cycle;

    if (cycle >= 341)
    {
        cycle = 0;
        ++scanline;

        if (scanline >= 261)
        {
            scanline = -1;
            // frameDrawn = true;
        }
    }
}


void RicohRP2C02::reset()
{
	fine_x = 0x00;
	addrLatch = 0x01;
	dataBuffer = 0x00;
	scanline = 0;
	cycle = 0;
	bg_next_tile_id = 0x00;
	bg_next_tile_attrib = 0x00;
	bg_next_tile_lsb = 0x00;
	bg_next_tile_msb = 0x00;
	bg_shifter_pattern_lo = 0x0000;
	bg_shifter_pattern_hi = 0x0000;
	bg_shifter_attrib_lo = 0x0000;
	bg_shifter_attrib_hi = 0x0000;
	statusRegister.raw = 0x00;
	maskRegister.raw = 0x00;
	controlRegister.raw = 0x00;
	vramAddr.raw = 0x0000;
	tramAddr.raw = 0x0000;
}