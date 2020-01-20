#pragma once
#include <cstdint>
#include <array>
#include <vector>

#include <HwConstants.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define TOP_MARGIN DISPLAY::PixelDim * 30
#define BOT_MARGIN DISPLAY::PixelDim * 120
#define LEFT_MARGIN DISPLAY::PixelDim * 10
#define RIGHT_MARGIN DISPLAY::PixelDim * 10

class Display
{
public:
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    SDL_Texture *text;

    const SDL_Rect canvas;
    const SDL_Rect controller;

    const uint32_t *frameBuffer;

    Display(const uint16_t width, const uint16_t height, const uint32_t *fb);
    ~Display() noexcept;

    void blit();
};