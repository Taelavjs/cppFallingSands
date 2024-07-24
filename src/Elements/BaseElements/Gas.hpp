#ifndef GAS_HPP
#define GAS_HPP
#include "SolidDynamic.hpp"

class Gas : public SolidDynamic {
public:    
    Gas();
    virtual ~Gas();
    int getBlocksToFall();
    void update(std::vector<std::vector<Pixel *>> &vec, int &row, int &col, int &vecWidth, int &vecHeight);
    bool isGas();
    void xDisp(std::vector<std::vector<Pixel *>> &vec, int row, int col, int xDispersion, int xDirection, int &res);

private:
    int yVelocity{0};
    int xVelocity{0};
    int maxBlocksToFall{25};
    int minDispersionRate{1};
    int maxDispersionRate{6};
    float verticalToHorizontalRation{0.4};
};
#endif /* GAS_HPP */
