#include "Pixel.hpp"
#include <algorithm>
#include <iostream>

Pixel::Pixel(){}
Pixel::~Pixel(){}

void Pixel::swapElements(Pixel* pix1, Pixel* pix2)
{
    *pix1 = *pix2;  
    pix2 = nullptr;
}

// FIRE SECTIONß

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
