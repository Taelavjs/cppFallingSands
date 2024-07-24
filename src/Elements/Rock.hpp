#ifndef ROCK_HPP
#define ROCK_HPP
#include "BaseElements/SolidImmoveable.hpp"
#include "BaseElements/Pixel.hpp"
#include <iostream>

class Rock : public SolidImmoveable {
public:
    Rock();
    virtual ~Rock();
    virtual Pixel *clone() const { return new Rock(); }
    virtual uint32_t getColour() { 
        return SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32), 200, 220, 150, 255); 
        }
};

#endif /* ROCK_HPP */
