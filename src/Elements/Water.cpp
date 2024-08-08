#include "Water.hpp"

Water::Water()
{
    setMass(8);
    movingRight = (randomNumber() > 0.5f);
    setHp(10);
}

Water::~Water() {}

uint32_t Water::getColour() {
    return SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32), 0, 50, 180, 200);
}

