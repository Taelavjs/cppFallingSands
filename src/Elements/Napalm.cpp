#include "Napalm.hpp";

uint32_t Napalm::getColour(){
    return SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32), 255, 50, 25, 200);
}

Napalm::Napalm()
{
    mass=8;
    movingRight = (randomNumber() > 0.5f);
}

Napalm::~Napalm() {}
