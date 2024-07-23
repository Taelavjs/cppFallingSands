#include "SolidDynamic.hpp"

SolidDynamic::SolidDynamic(){}
SolidDynamic::~SolidDynamic(){}

void SolidDynamic::updateVelocity() {
    velocity += g/3;
}

void SolidDynamic::resetVelocity() {
    velocity = 0;
}

int SolidDynamic::getBlocksToFall() {
    int acceleration = g / mass;
    velocity += acceleration;
    int blocksToFall = std::max(1, std::min(25, velocity / 2)); // Divide by a higher value to reduce the falling speed
    return blocksToFall;
}

double SolidDynamic::randomNumber(){
    static std::default_random_engine rng(static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count()));
    
    // Create a uniform distribution between 0.0 and 1.0
    static std::uniform_real_distribution<double> dist(0.0, 1.0);
    
    // Return a random double value in the range [0.0, 1.0)
    return dist(rng);

}

void SolidDynamic::swapElements(std::vector<std::vector<Pixel *>> &vec, 
                                int x1, int y1, 
                                int x2, int y2){
    Pixel *temp = vec[x1][y1];

    vec[x1][y1] = vec[x2][y2];
    vec[x2][y2] = temp;
}

void SolidDynamic::setProcessed(bool tf){
    isProcessed = tf;
}

bool SolidDynamic::getProcessed(){
    return isProcessed;
}
