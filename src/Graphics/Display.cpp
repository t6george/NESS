#include <Display.hpp>

Display::Display(const uint16_t width, const uint16_t height, const uint32_t *fb)
    :
    window{SDL_CreateWindow(
          "NESS", SDL_WINDOWPOS_UNDEFINED,
          SDL_WINDOWPOS_UNDEFINED,
          width * DISPLAY::PixelDim + LEFT_MARGIN + RIGHT_MARGIN,
          height * DISPLAY::PixelDim + TOP_MARGIN + BOT_MARGIN,
          SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN)},
    renderer{SDL_CreateRenderer(
          window, -1,
          SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)},
    texture{SDL_CreateTexture(
          renderer,
          SDL_PIXELFORMAT_ARGB8888,
          SDL_TEXTUREACCESS_STREAMING,
          width,
          height)},
      controllerTexture{SDL_CreateTextureFromSurface(renderer, IMG_Load("controller.png"))},

      canvas{.x = LEFT_MARGIN, .y = TOP_MARGIN, .w = width * DISPLAY::PixelDim, .h = height * DISPLAY::PixelDim},
      controller{.x = LEFT_MARGIN * 4, .y = canvas.h + TOP_MARGIN * 2, .w = canvas.w - LEFT_MARGIN * 6, .h = static_cast<int>((canvas.w - LEFT_MARGIN * 6) / 2.25)},
      frameBuffer{fb},
      buttonCoords
      {
          std::make_pair(419,719), std::make_pair(368,719),
          std::make_pair(130,699), std::make_pair(176,699),
          std::make_pair(153,678), std::make_pair(153,720),
          std::make_pair(249,718), std::make_pair(302,718)
      } {}

Display::~Display() noexcept
{
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Display::blit()
{
    SDL_SetRenderDrawColor(renderer, 0x40, 0x40, 0x40, 0xFF);
    SDL_UpdateTexture(texture, nullptr, frameBuffer, DISPLAY::Width * sizeof(uint32_t));
    SDL_RenderClear(renderer);

    SDL_RenderCopy(renderer, texture, nullptr, &canvas);
    SDL_RenderCopy(renderer, controllerTexture, nullptr, &controller);
    updateButtonPress(0);

    SDL_RenderPresent(renderer);
}

void Display::updateButtonPress(uint8_t activePress)
{
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0x00, 0xFF);

    for (int i = 0; i < 8; ++i)
    {
    for (int w = 0; w < RADIUS * 2; ++w)
    {
        for (int h = 0; h < RADIUS * 2; ++h)
        {
            int dx = RADIUS - w; // horizontal offset
            int dy = RADIUS - h; // vertical offset
            if ((dx * dx + dy * dy) <= (RADIUS * RADIUS))
            {
                SDL_RenderDrawPoint(renderer, buttonCoords[i].first + dx, buttonCoords[i].second + dy);
            }
        }
    }
    }  

}