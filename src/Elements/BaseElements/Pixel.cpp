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

bool Pixel::fireTick(WorldGeneration& worldGeneration, int row, int col){
    if(getOnFire()){
        for(int i = -1; i <= 1; ++i){
            for(int j = -1; j <= 1; ++j ){
                if(j ==0 && i == 0) continue;
                if(j == 1 && (i == 1 || i == -1)) continue;
                if(j == -1 && (i == 1 || i == -1)) continue;
                if(i == 1 && (j == 1 || j == -1)) continue;
                if(i == -1 && (j == 1 || j == -1)) continue;
                if(row+i > 384 || row+i < 0 || col+j > 384|| col+j < 0) continue;
                Pixel *ptrToNeighbor = worldGeneration.getPixelFromGlobal(Vector2D(col + i, row + j));
                if(ptrToNeighbor != nullptr && ptrToNeighbor->getIsFlammable() && !ptrToNeighbor->getOnFire()){
                    ptrToNeighbor->ignite();
                } else if(ptrToNeighbor != nullptr && ptrToNeighbor-> getIsLiquid() && !ptrToNeighbor->getIsFlammable()){
                    if(ptrToNeighbor->hit()){
                        int newRow = row + j;
                        int newCol = col + i;
                        worldGeneration.burntSmoke(newRow, newCol);
                    }
                }
            }
        }

        return hit();
    } 
    return false;
}
