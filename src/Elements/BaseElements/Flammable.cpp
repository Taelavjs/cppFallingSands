#include "Flammable.hpp"

Flammable::Flammable(){}
Flammable::~Flammable(){}

void Flammable::ignite(){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 1); 

    if(dis(gen) > chanceToIgnite){
        setOnFire();
    }
}

void Flammable::fireTick(){
    if(getOnFire()){
        hit();
    }
}
