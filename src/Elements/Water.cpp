#include "Water.hpp"

Water::Water()
{
    movingRight = (randomNumber() > 0.5f);
    density = 2;
}

Water::~Water() {}

uint32_t Water::getColour() {
    return SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32), 0, 50, 180, 200);
}

