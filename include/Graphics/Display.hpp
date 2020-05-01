#pragma once
#include <cstdint>
#include <array>
#include <vector>
#include <memory>

#include <HwConstants.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#define TOP_MARGIN DISPLAY::PixelDim * 30
#define BOT_MARGIN DISPLAY::PixelDim * 125
#define LEFT_MARGIN DISPLAY::PixelDim * 10
#define RIGHT_MARGIN DISPLAY::PixelDim * 10
#define RADIUS 10

class FileExplorer;
class GamePad;

class Display
{
    std::shared_ptr<GamePad> p1Controller;

    inline void drawButtonPress(const uint8_t buttonI);
    inline void drawCartridgeSlot();

    inline void drawCircle(const std::pair<uint16_t, uint16_t> center,
                           const uint16_t radius, const SDL_Color &col);

public:
    SDL_Window *window;
    SDL_Renderer *renderer;

    SDL_Texture *wallpaperTexture;
    SDL_Texture *texture;
    SDL_Texture *controllerTexture;
    SDL_Texture *menuTexture;

    const SDL_Rect canvas;
    const SDL_Rect controller;
    // const SDL_Rect slot;

    FileExplorer *fileExplorer;

    const uint32_t *frameBuffer;

    std::array<std::pair<uint16_t, uint16_t>, 0x8> buttonCoords;

    Display(const uint16_t width, const uint16_t height, const uint32_t *fb, std::shared_ptr<GamePad> p1);
    ~Display() noexcept;

    void blit();
    void setActiveButtons(const uint8_t activePress);
};