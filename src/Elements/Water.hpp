#ifndef WATER_hpp
#define WATER_hpp
#include "BaseElements/SolidDynamic.hpp"
#include "BaseElements/Pixel.hpp"
#include "BaseElements/Liquid.hpp"

class Water : public Liquid {
public:
    Water();
    virtual ~Water();
    virtual void update(std::vector<std::vector<Pixel *>> &vec, int &row, int &col, int &vecWidth, int &vecHeight) override;  
    virtual Pixel *clone() const { return new Water(); }
    virtual uint32_t getColour() { return SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32), 100, 100, 100, 255); }
    void moveHorizontally(int &vecWidth, std::vector<std::vector<Pixel *>> &vec, int col, int row, int incrementor);
private:
    bool movingRight{false};
};

#endif /* WATER_hpp */
