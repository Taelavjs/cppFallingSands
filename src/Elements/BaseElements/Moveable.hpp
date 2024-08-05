#ifndef MOVEABLE_HPP
#define MOVEABLE_HPP
#include "Pixel.hpp"
#include <math.h>
#include <random>


class Moveable : public Pixel {
public:
    double randomNumber();
    void updateVelocity(int &newCol, int upDown);
    bool isMoveable() {
        return true; 
    }  
    void resetVelocity(){
        yVelocity = 0;
    }
    int getDensity(){return density;}
    bool isSpaceFree(std::vector<std::vector<Pixel *>> &vec, int row, int col);
private:
    int yVelocity{0};
    int xVelocity{0};
    int maxBlocksToFall{25};
    int minDispersionRate{1};
    int maxDispersionRate{6};
    float verticalToHorizontalRation{0.4};
};

#endif /* MOVEABLE_HPP */