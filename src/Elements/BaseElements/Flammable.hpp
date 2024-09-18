#ifndef FLAMMABLE_HPP
#define FLAMMABLE_HPP
#include <random>
#include <vector>
#include "Pixel.hpp"
#include "../../GameSetup/World/WorldGeneration.hpp" 

class Flammable {
public:
    Flammable();
    ~Flammable();
    void ignite();
    int hit() {hp--; return hp <= 0;}
    void fireTick(Chunk &vec, int row, int col);
    bool getOnFire(){return onFire;};
    void setOnFire(){onFire = true;};
private:
    int hp{5};
    double chanceToIgnite{0.5f};
    bool onFire{false};
};
#endif /* FLAMMABLE_HPP */
