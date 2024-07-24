#ifndef SOLIDDYNAMIC_HPP
#define SOLIDDYNAMIC_HPP
#include "Pixel.hpp"
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <random>
class SolidDynamic : public Pixel {
protected:    
    SolidDynamic();
    virtual ~SolidDynamic();
    void updateVelocity();
    void resetVelocity();
    int getBlocksToFall();
    double randomNumber();
    void swapElements(std::vector<std::vector<Pixel *>> &vec, int x1, int y1, int x2, int y2);
    void setProcessed(bool tf) ;
    bool getProcessed() ;
    virtual bool isMoveable(){return true;}
    void transferVelocityX();

private:
    int yVelocity{0};
    int xVelocity{0};
    int maxBlocksToFall{25};
    int minDispersionRate{1};
    int maxDispersionRate{6};
    float verticalToHorizontalRation{0.4};
};


#endif /* SOLIDDYNAMIC_HPP */
