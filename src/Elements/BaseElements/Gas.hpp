#ifndef GAS_HPP
#define GAS_HPP
#include "SolidDynamic.hpp"
#include "Pixel.hpp"
#include "Moveable.hpp"
#include <cmath>

class Gas : public Moveable {
public:    
    Gas();
    virtual ~Gas();
    virtual void update(int row, int col, int &vecWidth, int &vecHeight, WorldGeneration &worldGeneration);
    bool isGas();
    bool isSolid(){return false;};
    bool isMoveable(){return true;};
private:
    int randomDir{0};
    int yVelocity{0};

    int x_direction{0};
};
#endif /* GAS_HPP */
