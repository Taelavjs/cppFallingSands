#ifndef FLAMMABLE_HPP
#define FLAMMABLE_HPP

class Flammable {
public:
    Flammable();
    ~Flammable();

    int hit() {hp--; return hp <= 0;}
private:
    int hp{5};
    double chanceToIgnite{0.5f};
};
#endif /* FLAMMABLE_HPP */
