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
    int getBlocksToFall();
    virtual void update(int row, int col, int &vecWidth, int &vecHeight, WorldGeneration &worldGeneration);
    bool isGas();
    void xDisp(Chunk &vec, int row, int col, int xDispersion, int xDirection, int &res);
    bool isSolid(){return false;};
    int calculateNewColumn(const Chunk& vec, int row, int col, int xDispersion, int xDirection);
    bool isValidPosition(Chunk& vec, int row, int col);
    void updateYPosition(int &newCol);
    bool isMoveable(){return true;};
private:
    int randomDir{0};
    int yDispersion{7}; // Distance to check for dispersion
    int xDispersion{7}; // Distance to check for dispersion
    int yVelocity{0};

    int x_direction{0};
};
#endif /* GAS_HPP */
