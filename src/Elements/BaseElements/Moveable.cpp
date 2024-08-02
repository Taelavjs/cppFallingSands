#include "Moveable.hpp"

void Moveable::updateVelocity(int &newCol, int upDown)
{
    newCol += ((int)2.0f/getMass() + yVelocity) * upDown;
    yVelocity += (int)2.0f * upDown;
    if(yVelocity > terminalY * upDown) yVelocity = terminalY;
}

double Moveable::randomNumber()
{
    static std::default_random_engine rng(static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count()));

    // Create a uniform distribution between 0.0 and 1.0
    static std::uniform_real_distribution<double> dist(0.0, 1.0);

    // Return a random double value in the range [0.0, 1.0)
    return dist(rng);
}

bool Moveable::isSpaceFree(std::vector<std::vector<Pixel *>> &vec, int row, int col){
    return ( vec[row][col] == nullptr || vec[row][col]->isLiquid() || vec[row][col]-> isGas());
};
