#ifndef LIQUID_HPP
#define LIQUID_HPP
#include "Pixel.hpp"
#include <algorithm>

class Liquid:public Pixel {
public:
    Liquid();
    ~Liquid();
    double randomNumber();
    void setProcessed(bool tf) ;
    bool getProcessed() ;
    virtual bool isMoveable(){return true;}
    bool swapOnDensity(std::vector<std::vector<Pixel *>> &vec, int x1, int y1, int x2, int y2, int density1, int density2);
    int getDensity(){return density;}
    int getDispersionRate(){return 5;};
    virtual bool isLiquid();
    virtual void update(std::vector<std::vector<Pixel *>> &vec, int &row, int &col, int &vecWidth, int &vecHeight);
    void moveHorizontally(int &vecWidth, std::vector<std::vector<Pixel *>> &vec, int col, int row, int incrementor);

private:
    int maxDispersionRate{9};
    int minDispersionRate{3};
    float verticalToHorizontalRation{0.7f};
    int yVelocity{1};
    int maxBlocksToFall{25};
    int xVelocity{1};
    int x_direction{0};
    int xMaxDistance{5};

};


#endif /* LIQUID_HPP */
