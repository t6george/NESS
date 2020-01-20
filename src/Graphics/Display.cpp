#include <Display.hpp>

Display::Display(const uint16_t width, const uint16_t height, const uint32_t *fb)
    : window{SDL_CreateWindow(
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
      canvas{.x = LEFT_MARGIN, .y = TOP_MARGIN, .w = width * DISPLAY::PixelDim, .h = height * DISPLAY::PixelDim},
      controller{.x = LEFT_MARGIN * 4, .y = canvas.h + TOP_MARGIN * 2, .w = canvas.w - LEFT_MARGIN * 6, .h = static_cast<int>((canvas.w - LEFT_MARGIN * 6) / 2.5)},
      frameBuffer{fb}
{
    SDL_Surface *surface = IMG_Load("controller.png");
    text = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    SDL_SetRenderDrawColor(renderer, 0x40, 0x40, 0x40, 0xFF);
}

Display::~Display() noexcept
{
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Display::blit()
{
    SDL_UpdateTexture(texture, nullptr, frameBuffer, DISPLAY::Width * sizeof(uint32_t));
    SDL_RenderClear(renderer);

    SDL_RenderCopy(renderer, texture, nullptr, &canvas);
    SDL_RenderCopy(renderer, text, nullptr, &controller);

    // SDL_Rect controller{.x = LEFT_MARGIN * 4, .y = canvas.h + TOP_MARGIN * 2, .w = canvas.w - LEFT_MARGIN * 6, .h = (canvas.w - LEFT_MARGIN * 6) / 2};
    // SDL_RenderFillRect(renderer, &controller);

    // SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
    // int radius = 20;

    // for (int w = 0; w < radius * 2; w++)
    // {
    //     for (int h = 0; h < radius * 2; h++)
    //     {
    //         int dx = radius - w; // horizontal offset
    //         int dy = radius - h; // vertical offset
    //         if ((dx * dx + dy * dy) <= (radius * radius))
    //         {
    //             SDL_RenderDrawPoint(renderer, 350 + dx, 680 + dy);
    //         }
    //     }
    // }

    SDL_RenderPresent(renderer);
}