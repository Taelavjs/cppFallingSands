#include "Smoke.hpp"

#include "Water.hpp"

Smoke::Smoke()
{
    setIsGas(true);
    setColor(SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32), 130 + randomNumber(), 130+randomNumber(), 130, 255));
}

Smoke::~Smoke() {}
