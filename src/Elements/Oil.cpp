#include "Oil.hpp"

Oil::Oil()
{
    mass = 14;
    setFlammable(true);
}

Oil::~Oil() {}

uint32_t Oil::getColour() {
    return SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32), 90, 50, 30, 200);
}

