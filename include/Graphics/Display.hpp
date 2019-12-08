#pragma once
#include <cstdint>
#include <array>

#include <HwConstants.hpp>
#include <SDL2/SDL.h>

class Display
{
    bool frameDrawn;
    uint16_t scanline;
    uint16_t cycle;

    std::unique_ptr<SDL_Window> window;

    // std::array<uint8_t, DISPLAY::ColorCount> colors;

public:
    Display(const uint16_t width, const uint16_t height);
    ~Display() = default;
};