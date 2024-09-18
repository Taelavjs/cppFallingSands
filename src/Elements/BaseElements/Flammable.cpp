#include "Flammable.hpp"

Flammable::Flammable(){}
Flammable::~Flammable(){}

void Flammable::ignite(){
    if(getOnFire()) return; // already ignited
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 1); 

    if(dis(gen) > chanceToIgnite){
        setOnFire();
    }
}

void Flammable::fireTick(Chunk &vec, int row, int col){
    if(getOnFire()){
        for(int i = -1; i <= 1; ++i){
            for(int j = -1; j <= 1; ++j ){
                if(j==i) continue;
                if(vec[row+1][col+j]->getIsFlammable()){
                    vec[row+1][col+j]->ignite();
                }
            }
        }

        hit();
    }
}
