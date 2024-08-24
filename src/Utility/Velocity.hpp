#ifndef VELOCITY
#define VELOCITY
#include <iostream>
#include <tuple>

struct Vector2D {
    Vector2D(float xp, float yp){
        x = xp;
        y = yp;
    }
    float x{0};
    float y{0};
};

class Velocity {
public:
    Velocity();
    ~Velocity();

    Vector2D getVelocity();
    void velocityTick();
    void resetVelocity(){
        velocity.x = 0;
        velocity.y = 0;
    }

private:
    Vector2D velocity;
    const float gravity{9.8f};
    const int mass{5};

    const float maxYVelocity{5.0f};
    const float maxXVelocity{5.0f};

};

#endif /* VELOCITY */
