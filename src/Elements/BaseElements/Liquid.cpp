#include "Liquid.hpp"

#include "SolidDynamic.hpp"

Liquid::Liquid() {}
Liquid::~Liquid() {}

void Liquid::updateVelocity()
{
    // Constants for simulation
    const float drag = 0.98f;         // Drag factor (0 < drag < 1)
    const float maxVelocity = 30.0f;  // Terminal yVelocity

    // Update the yVelocity with gravity
    yVelocity += g / 3;

    // Apply drag to the yVelocity
    yVelocity *= drag;
    xVelocity *= drag;

    // Ensure yVelocity does not exceed terminal yVelocity
    if (yVelocity > maxVelocity)
    {
        yVelocity = maxVelocity;
    }
}

void Liquid::transferVelocityX(){
    xVelocity += std::max(minDispersionRate, std::min(static_cast<int>(verticalToHorizontalRation * yVelocity), maxDispersionRate));
    
}
bool Liquid::isLiquid(){
    return true;
}

void Liquid::resetVelocity()
{
    transferVelocityX();

    yVelocity = 0;
}

int Liquid::getBlocksToFall()
{
    int acceleration = g / mass;
    yVelocity += acceleration;
    int blocksToFall = std::max(1, std::min(25, yVelocity / 2)); // Divide by a higher value to reduce the falling speed
    return blocksToFall;
}

double Liquid::randomNumber()
{
    static std::default_random_engine rng(static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count()));

    // Create a uniform distribution between 0.0 and 1.0
    static std::uniform_real_distribution<double> dist(0.0, 1.0);

    // Return a random double value in the range [0.0, 1.0)
    return dist(rng);
}

void Liquid::swapElements(std::vector<std::vector<Pixel *>> &vec,
                                int x1, int y1,
                                int x2, int y2)
{
    Pixel *temp = vec[x1][y1];

    vec[x1][y1] = vec[x2][y2];
    vec[x2][y2] = temp;
}

bool Liquid::swapOnDensity(std::vector<std::vector<Pixel *>> &vec,
                                 int x1, int y1,
                                 int x2, int y2, int density1, int density2)
{
    if(density1 < density2){
        Pixel *temp = vec[x1][y1];
        vec[x1][y1] = vec[x2][y2];
        vec[x2][y2] = temp;
        return true;
    }
    return false;


}

void Liquid::setProcessed(bool tf)
{
    isProcessed = tf;
}

bool Liquid::getProcessed()
{
    return isProcessed;
}
