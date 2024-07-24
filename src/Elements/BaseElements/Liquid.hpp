#ifndef LIQUID_HPP
#define LIQUID_HPP
#include "Pixel.hpp"
#include <algorithm>

class Liquid:public Pixel {
public:
    Liquid();
    ~Liquid();
    virtual void update(std::vector<std::vector<Pixel *>> &vec, int &row, int &col, int &vecWidth, int &vecHeight) {};
    void updateVelocity();
    void resetVelocity();
    int getBlocksToFall();
    double randomNumber();
    void swapElements(std::vector<std::vector<Pixel *>> &vec, int x1, int y1, int x2, int y2);
    void setProcessed(bool tf) ;
    bool getProcessed() ;
    virtual bool isMoveable(){return true;}
    bool swapOnDensity(std::vector<std::vector<Pixel *>> &vec, int x1, int y1, int x2, int y2, int density1, int density2);
    int getDensity(){return density;}
    int getDispersionRate(){return xVelocity;};
    void transferVelocityX();
private:
    int maxDispersionRate{5};
    int yVelocity{0};
    int maxBlocksToFall{25};
    int xVelocity{0};

};


#endif /* LIQUID_HPP */
