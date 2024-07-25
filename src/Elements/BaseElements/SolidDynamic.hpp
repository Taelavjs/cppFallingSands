#ifndef SOLIDDYNAMIC_HPP
#define SOLIDDYNAMIC_HPP
#include "Pixel.hpp"
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <random>
class SolidDynamic : public Pixel {
public:
    bool isSolid(){return true;};
    bool isMoveable() {
        return true; // Or whatever logic you want for SolidDynamic
    }  
protected:    
    SolidDynamic();
    virtual ~SolidDynamic();
    virtual void updateVelocity();
    void resetVelocity();
    virtual int getBlocksToFall();
    double randomNumber();
    void swapElements(std::vector<std::vector<Pixel *>> &vec, int x1, int y1, int x2, int y2);
    void setProcessed(bool tf) ;
    bool getProcessed() ;
  
    virtual void transferVelocityX();
    virtual void update(std::vector<std::vector<Pixel *>> &vec, int &row, int &col, int &vecWidth, int &vecHeight);
    virtual void xDisperse(std::vector<std::vector<Pixel *>> &vec, int row, int col, int xDispersion, int xDirection, int &res);
    bool checkMoveableMat(Pixel *space);
private:
    int yVelocity{0};
    int xVelocity{0};
    int maxBlocksToFall{25};
    int minDispersionRate{1};
    int maxDispersionRate{6};
    float verticalToHorizontalRation{0.4};
};


#endif /* SOLIDDYNAMIC_HPP */
