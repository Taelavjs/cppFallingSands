#include <Velocity.hpp>

Velocity::Velocity():
    velocity(0.0f, 0.0f)    
{
}   

Vector2D Velocity::getVelocity(){

}

void Velocity::velocityTick(){
    velocity.y += gravity * mass;

    velocity.y = std::min(velocity.y, maxYVelocity);
    velocity.x = std::min(velocity.x, maxXVelocity);
}
