#ifndef WATER_hpp
#define WATER_hpp
#include "BaseElements/SolidDynamic.hpp"
#include "BaseElements/Pixel.hpp"
#include "BaseElements/Liquid.hpp"

class Water : public Liquid {
public:
    Water();
    virtual ~Water();
    virtual Pixel *clone() const { return new Water(); }
public:
    virtual uint32_t getColour();
private:
    bool movingRight{false};
};

#endif /* WATER_hpp */
