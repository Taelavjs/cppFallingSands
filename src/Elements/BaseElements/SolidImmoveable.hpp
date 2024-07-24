#ifndef SOLIDIMMOVEABLE_HPP
#define SOLIDIMMOVEABLE_HPP
#include "Pixel.hpp"

class SolidImmoveable : public Pixel {
public:
    SolidImmoveable(){};
    ~SolidImmoveable(){};
protected:
    virtual bool isMoveable(){return false;}
};

#endif /* SOLIDIMMOVEABLE_HPP */
