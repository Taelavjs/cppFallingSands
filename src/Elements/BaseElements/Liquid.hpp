#ifndef LIQUID_HPP
#define LIQUID_HPP
#include "Pixel.hpp"
#include "Moveable.hpp"
#include <algorithm>

class Liquid:public Moveable {
public:
    Liquid();
    ~Liquid();
    bool swapOnDensity(Chunk &vec, int x1, int y1, int x2, int y2, int density1, int density2);
    int getDispersionRate(){return 5;};
    virtual void update(int &row, int &col, int &vecWidth, int &vecHeight, WorldGeneration &worldGeneration);

    void moveHorizontally(int &vecWidth, Chunk &vec, int col, int row, int incrementor);
    int getMovingDirection();
private:
    int maxDispersionRate{9};
    int minDispersionRate{3};
    float verticalToHorizontalRation{0.7f};
    int yVelocity{1};
    int maxBlocksToFall{25};
    int xVelocity{1};
    int x_direction{0};
    int xMaxDistance{5};
    int density{5};
};


#endif /* LIQUID_HPP */
