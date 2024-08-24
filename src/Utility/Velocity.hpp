#ifndef VELOCITY
#define VELOCITY
#include <iostream>
#include <tuple>
#include <cmath>

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

    Vector2D getVelocity(){
        return velocity;
    };
    void velocityTick();
    void resetVelocity(){
        velocity.x = 0;
        velocity.y = 0;
    }

    void addForce(float vectorLength, float angle);
    void setIsGrounded(bool value){
        isGrounded = value;
    }

private:
    Vector2D velocity;
    const float gravity{1.1f};
    const float mass{1};

    const float maxYVelocity{1.5f};
    const float maxXVelocity{1.5f};

    bool isGrounded{false};

};

#endif /* VELOCITY */
