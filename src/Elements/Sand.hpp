#ifndef Sand_hpp
#define Sand_hpp
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <random>
#include "BaseElements/SolidDynamic.hpp"
#include "BaseElements/Pixel.hpp"

class Sand : public SolidDynamic {
public:
    Sand();
    virtual ~Sand();
    virtual Pixel *clone() const { return new Sand(); }
    virtual uint32_t getColour() { return SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32), 200, 200, 0, 255); }
};


#endif /* Sand_hpp */
