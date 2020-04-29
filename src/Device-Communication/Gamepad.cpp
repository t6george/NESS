#include <GamePad.hpp>

GamePad::GamePad() : buttonReg{0x00}, buttonState{0x00} {}

void GamePad::registerInputStateChange(const SDL_Event &e)
{
    switch (e.type)
    {
    case SDL_KEYDOWN:

        switch (e.key.keysym.sym)
        {
        case SDLK_LEFT: // D-pad
            buttonReg |= 0x02;
            break;
        case SDLK_RIGHT:
            buttonReg |= 0x01;
            break;
        case SDLK_UP:
            buttonReg |= 0x08;
            break;
        case SDLK_DOWN:
            buttonReg |= 0x04;
            break;
        case SDLK_a: // A
            buttonReg |= 0x80;
            break;
        case SDLK_s: // B
            buttonReg |= 0x40;
            break;
        case SDLK_z: // Start
            buttonReg |= 0x10;
            break;
        case SDLK_x: // Select
            buttonReg |= 0x20;
            break;
        default:
            break;
        }
        break;
    case SDL_KEYUP:
        switch (e.key.keysym.sym)
        {
        case SDLK_LEFT: // D-pad
            buttonReg &= ~0x02;
            break;
        case SDLK_RIGHT:
            buttonReg &= ~0x01;
            break;
        case SDLK_UP:
            buttonReg &= ~0x08;
            break;
        case SDLK_DOWN:
            buttonReg &= ~0x04;
            break;
        case SDLK_a: // A
            buttonReg &= ~0x80;
            break;
        case SDLK_s: // B
            buttonReg &= ~0x40;
            break;
        case SDLK_z: // Start
            buttonReg &= ~0x10;
            break;
        case SDLK_x: // Select
            buttonReg &= ~0x20;
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
}

void GamePad::writeButtonState()
{
    buttonState = buttonReg;
}

uint8_t GamePad::readStateMSB()
{
    uint8_t msb = (buttonState & 0x80) != 0x00;
    buttonState <<= 1;
    return msb;
}

uint8_t GamePad::readPressReg() const
{
    return buttonReg;
}