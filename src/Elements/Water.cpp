#include "Water.hpp"

Water::Water()
{
    movingRight = (randomNumber() > 0.5f);
    density = 2;
}

Water::~Water() {}

uint32_t Water::getColour() {
    int k = std::max(1, std::min(getSumVelocity(), 10));
    double k_adjusted = std::pow(k, 0.7); // Adjust exponent as needed
    int blue_value = std::min(255, static_cast<int>(120 + k_adjusted * 100)); // Increase max blue and adjust scaling
    return SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32), 0, 0, blue_value, 200);
}

