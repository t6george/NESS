#pragma once
#include <cstdint>
#include <array>
#include <vector>

#include <HwConstants.hpp>
#include <SDL2/SDL.h>

class Display
{
public:
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;

    const SDL_Rect canvas;

    const uint32_t *frameBuffer;

    Display(const uint16_t width, const uint16_t height, const uint32_t *fb);
    ~Display() noexcept;
    void blit();
};