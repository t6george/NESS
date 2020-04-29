#pragma once
#include <cstdint>
#include <SDL2/SDL.h>

class GamePad
{
    uint8_t buttonReg;
    uint8_t buttonState;

public:
    GamePad();
    ~GamePad() = default;
    void registerInputStateChange(const SDL_Event &e);
    void writeButtonState();
    uint8_t readStateMSB();
    uint8_t readPressReg() const;
};