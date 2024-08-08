#ifndef Oil_HPP
#define Oil_HPP
#include "BaseElements/Flammable.hpp"
#include "BaseElements/Liquid.hpp"

class Oil : public Liquid {
public:
    Oil();
    virtual ~Oil();
    virtual Pixel *clone() const { return new Oil(); }

public:
    virtual uint32_t getColour();
    
};  

#endif /* Oil_HPP */
