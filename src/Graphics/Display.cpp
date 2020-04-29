#include <Display.hpp>
#include <FileExplorer.hpp>
#include <GamePad.hpp>

Display::Display(const uint16_t width, const uint16_t height, const uint32_t *fb, std::shared_ptr<GamePad> p1)
    : p1Controller{p1},
      window{SDL_CreateWindow(
          "NESS", SDL_WINDOWPOS_UNDEFINED,
          SDL_WINDOWPOS_UNDEFINED,
          width * DISPLAY::PixelDim + LEFT_MARGIN + RIGHT_MARGIN,
          height * DISPLAY::PixelDim + TOP_MARGIN + BOT_MARGIN,
          SDL_WINDOW_SHOWN)},
      renderer{SDL_CreateRenderer(
          window, -1,
          SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)},
      wallpaperTexture{SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/bg.jpg"))},
      texture{SDL_CreateTexture(
          renderer,
          SDL_PIXELFORMAT_ARGB8888,
          SDL_TEXTUREACCESS_STREAMING,
          width,
          height)},
      controllerTexture{SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/controller.png"))},
      menuTexture{nullptr},
      canvas{.x = LEFT_MARGIN, .y = TOP_MARGIN, .w = width * DISPLAY::PixelDim, .h = height * DISPLAY::PixelDim},
      controller{.x = LEFT_MARGIN * 4, .y = canvas.h + TOP_MARGIN * 2, .w = canvas.w - LEFT_MARGIN * 6, .h = static_cast<int>((canvas.w - LEFT_MARGIN * 6) / 2.25)},
      //   slot{.x = LEFT_MARGIN * 6, .y = TOP_MARGIN / 4, .w = canvas.w - LEFT_MARGIN * 10, .h = TOP_MARGIN / 2},
      //   fileMenu{.x = LEFT_MARGIN * 6, .y = TOP_MARGIN / 2, .w = canvas.w - LEFT_MARGIN * 10, .h = TOP_MARGIN / 2},
      fileExplorer{new FileExplorer()}, frameBuffer{fb},
      buttonCoords{
          std::make_pair(176, 699),
          std::make_pair(130, 699),
          std::make_pair(153, 720),
          std::make_pair(153, 678),
          std::make_pair(302, 718),
          std::make_pair(249, 718),
          std::make_pair(419, 719),
          std::make_pair(368, 719),
      }
{
}

Display::~Display() noexcept
{
    delete fileExplorer;
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}

void Display::setActiveButtons(const uint8_t activePress)
{
    if (activePress != 0)
    {
        if (activePress & 0x1)
            drawButtonPress(0x0);

        if (activePress & 0x2)
            drawButtonPress(0x1);

        if (activePress & 0x4)
            drawButtonPress(0x2);

        if (activePress & 0x8)
            drawButtonPress(0x3);

        if (activePress & 0x10)
            drawButtonPress(0x4);

        if (activePress & 0x20)
            drawButtonPress(0x5);

        if (activePress & 0x40)
            drawButtonPress(0x6);

        if (activePress & 0x80)
            drawButtonPress(0x7);
    }
}

void Display::drawButtonPress(const uint8_t buttonI)
{
    drawCircle(buttonCoords[buttonI], RADIUS, {.r = 0xFF, .g = 0xFF, .b = 0x00, .a = 0xFF});
}

void Display::drawCartridgeSlot()
{
    SDL_SetRenderDrawColor(renderer, 0x20, 0x20, 0x20, 0xFF);
    // SDL_RenderFillRect(renderer, &slot);
    SDL_Color col = {.r = 0xFF, .g = 0xFF, .b = 0xFF, .a = 0xFF};
    std::pair<uint16_t, uint16_t> coords;
    for (uint8_t i = 0; i < 30; ++i)
    {
        coords = {LEFT_MARGIN * 6 + 10 + i * 10, TOP_MARGIN / 4 + 10};
        drawCircle(coords, 2, col);
        coords.second += 10;
        drawCircle(coords, 2, col);
    }
}

void Display::drawCircle(const std::pair<uint16_t, uint16_t> center,
                         const uint16_t radius, const SDL_Color &col)
{
    int16_t dx, dy;
    SDL_SetRenderDrawColor(renderer, col.r, col.g, col.b, col.a);
    for (uint16_t w = 0; w < radius * 2; ++w)
    {
        for (uint16_t h = 0; h < radius * 2; ++h)
        {
            dx = radius - w;
            dy = radius - h;
            if ((dx * dx + dy * dy) <= (radius * radius))
            {
                SDL_RenderDrawPoint(renderer, center.first + dx, center.second + dy);
            }
        }
    }
}

void Display::blit()
{
    const uint8_t activePress = p1Controller->readPressReg();

    SDL_UpdateTexture(texture, nullptr, frameBuffer, DISPLAY::Width * sizeof(uint32_t));
    SDL_RenderClear(renderer);

    SDL_RenderCopy(renderer, wallpaperTexture, nullptr, nullptr);

    SDL_RenderCopy(renderer, texture, nullptr, &canvas);

    SDL_RenderCopy(renderer, controllerTexture, nullptr, &controller);
    // drawCartridgeSlot();

    setActiveButtons(activePress);

    SDL_RenderPresent(renderer);
}