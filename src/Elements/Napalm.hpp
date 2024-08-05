#ifndef NAPALM_HPP
#define NAPALM_HPP
#include "BaseElements/Liquid.hpp";
#include "BaseElements/Pixel.hpp";
#include "BaseElements/Flammable.hpp";

class Napalm : public Liquid, public Flammable {
public:
    Napalm();
    virtual ~Napalm();
    virtual Pixel *clone() const { return new Napalm(); }
    int getDensity(){return 3;};

public:
    virtual uint32_t getColour();
private:
    bool movingRight{false};
};

#endif /* NAPALM_HPP */
