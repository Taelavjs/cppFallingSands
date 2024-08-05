#include "Pixel.hpp"
#include <algorithm>
#include <iostream>

Pixel::Pixel(){}
Pixel::~Pixel(){}

bool Pixel::isLiquid(){
    return false;
}

bool Pixel::isMoveable(){
    return moveable;
}

void Pixel::swapElements(std::vector<std::vector<Pixel*>>& vec,
                                int x1, int y1,
                                int x2, int y2)
{


    if (x1 != x2) {
        // Swap elements in different rows
        Pixel* temp = vec[x1][y1];
        vec[x1][y1] = vec[x2][y2];
        vec[x2][y2] = temp;
        return;
    }

    // Swap elements horizontally within the same row
    if (y1 > y2) {
        for (int y = y2; y < y1; ++y) {
            Pixel* temp = vec[x1][y];
            vec[x1][y] = vec[x1][y+1];
            vec[x1][y+1] = temp;
        }
    } else {
        for (int y = y2; y > y1; --y) {
            Pixel* temp = vec[x1][y];
            vec[x1][y] = vec[x1][y-1];
            vec[x1][y-1] = temp;
        }
    }
}

// FIRE SECTION

void Pixel::ignite(){};
int Pixel::hit() {};
void Pixel::fireTick(std::vector<std::vector<Pixel *>> &vec, int row, int col){};
bool Pixel::getOnFire(){return false;};
void Pixel::setOnFire(){};
