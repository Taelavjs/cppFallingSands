#ifndef WATER_hpp
#define WATER_hpp
#include "BaseElements/SolidDynamic.hpp"

class Water : public SolidDynamic {
public:
    Water();
    virtual ~Water();
    void update(std::vector<std::vector<Pixel *>> &vec, int &row, int &col, int &vecWidth, int &vecHeight);  

private:
    bool movingRight{false};
};

#endif /* WATER_hpp */
