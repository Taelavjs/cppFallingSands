#ifndef SOLIDIMMOVEABLE_HPP
#define SOLIDIMMOVEABLE_HPP
#include "Pixel.hpp"

class SolidImmoveable : public Pixel {
public:
    SolidImmoveable(){moveable = false;};
    ~SolidImmoveable(){};
};

#endif /* SOLIDIMMOVEABLE_HPP */
