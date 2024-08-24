#include "Velocity.hpp"

Velocity::Velocity():
    velocity(0.0f, 0.0f)    
{
}   
Velocity::~Velocity(){

}


void Velocity::velocityTick(){
    if(!isGrounded){
        velocity.y -= gravity * mass;
        velocity.x *= 0.95f;
    } else {
        velocity.y = 0;
        velocity.x *= 0.6f;
    }
    if(velocity.y > 0){
        velocity.y = std::min(velocity.y, maxYVelocity);

        if(velocity.y < 0.3){
            velocity.y = 0;
        }
    } else if(velocity.y < 0) {
        velocity.y = std::max(velocity.y, -maxYVelocity);
        if(velocity.y > -0.3){
            velocity.y = 0;
        }
    }
    if(velocity.x > 0){
        velocity.x = std::min(velocity.x, maxXVelocity);
        if(velocity.x < 0.3){
            velocity.x = 0;
        }
    } else if(velocity.x < 0) {
        velocity.x = std::max(velocity.x, -maxXVelocity);
        if(velocity.x > -0.3){
            velocity.x = 0;
        }
    }


    std::cout << "Velocity X : " << velocity.x << '\n';
    std::cout << "Velocity Y : " << velocity.y << '\n';
}

void Velocity::addForce(float vectorLength, float angle){
    float radianAngle = angle * (M_PI / 180.0f);
    if(isGrounded){
        velocity.x += std::cos(radianAngle) * vectorLength * 0.7f;
        velocity.y += std::sin(radianAngle) * vectorLength * 0.7f;
    } else {
        velocity.x += std::cos(radianAngle) * vectorLength * 0.3f;
        velocity.y += std::sin(270) * 10 * 0.7f;

    }

}
