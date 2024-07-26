#ifndef FLAMMABLE_HPP
#define FLAMMABLE_HPP

class Flammable {
public:
    Flammable();
    ~Flammable();
private:
    int hp{5};
    double chanceToIgnite{0.5f};
    
};
#endif /* FLAMMABLE_HPP */
