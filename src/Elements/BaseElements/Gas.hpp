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
    void update(std::vector<std::vector<Pixel *>> &vec, int &row, int &col, int &vecWidth, int &vecHeight);
    bool isGas();
    void xDisp(std::vector<std::vector<Pixel *>> &vec, int row, int col, int xDispersion, int xDirection, int &res);
    bool isSolid(){return false;};
    int calculateNewColumn(const std::vector<std::vector<Pixel*>>& vec, int row, int col, int xDispersion, int xDirection);
    bool isValidPosition(std::vector<std::vector<Pixel*>>& vec, int row, int col);
    void updateYPosition(int &newCol);
    bool isMoveable(){return true;};
private:
    int randomDir{0};
    int yDispersion{7}; // Distance to check for dispersion
    int xDispersion{15}; // Distance to check for dispersion
    int yVelocity{0};
};
#endif /* GAS_HPP */
