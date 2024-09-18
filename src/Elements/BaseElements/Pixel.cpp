#include "Pixel.hpp"
#include <algorithm>
#include <iostream>

Pixel::Pixel(){}
Pixel::~Pixel(){}

void Pixel::swapElements(Chunk& vec,
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

bool Pixel::getOnFire(){return onFire;};
void Pixel::setOnFire(){
    onFire = true;
};


void Pixel::ignite(){
    if(getOnFire()) return; // already ignited
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    if(dis(gen) > chanceToIgnite){
        setOnFire();
    }
}
bool Pixel::hit() {
    hp--; 
    return hp <= 0;
}

bool Pixel::fireTick(Chunk &vec, int row, int col, int vecHeight, Pixel *elm){
    if(getOnFire()){
        for(int i = -1; i <= 1; ++i){
            for(int j = -1; j <= 1; ++j ){
                if(j==i) continue;
                if(row+i > vecHeight -1 || row+i < 0 || col+j > vecHeight -1 || col+j < 0) continue;
                Pixel *ptrToNeighbor = vec[row+i][col+j];
                if(ptrToNeighbor != nullptr && ptrToNeighbor->getIsFlammable() && !ptrToNeighbor-> getOnFire()){
                    vec[row+i][col+j]->ignite();
                } else if(ptrToNeighbor != nullptr && ptrToNeighbor-> getIsLiquid() && !ptrToNeighbor->getIsFlammable()){
                    if(ptrToNeighbor->hit()){
                        vec[row+i][col+j] = elm->clone();
                    }
                }
            }
        }

        return hit();
    } 
    return false;
}
