#ifndef FLAMMABLE_HPP
#define FLAMMABLE_HPP
#include <random>

class Flammable {
public:
    Flammable();
    ~Flammable();
    void fireTick();
    int hit() {hp--; return hp <= 0;}
    void ignite();
    bool getOnFire(){return onFire;};
    void setOnFire(){onFire = true;};
private:
    int hp{5};
    double chanceToIgnite{0.5f};
    bool onFire{false};
};
#endif /* FLAMMABLE_HPP */
