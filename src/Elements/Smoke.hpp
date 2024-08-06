#ifndef SMOKE_HPP
#define SMOKE_HPP
#include "BaseElements/Gas.hpp"
#include "BaseElements/Pixel.hpp"
class Smoke : public Gas {
public:
    Smoke();
    virtual ~Smoke();
    virtual Pixel *clone() const { return new Smoke(); }
    virtual uint32_t getColour() { return SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32), 30 + randomNumber(), 30+randomNumber(), 0, 200); }
};
#endif /* SMOKE_HPP*/
