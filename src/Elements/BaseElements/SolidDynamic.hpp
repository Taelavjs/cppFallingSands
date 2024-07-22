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
    SolidDynamic();
    virtual ~SolidDynamic();
    virtual void update(std::vector<std::vector<Pixel *>> &vec, int &row, int &col, int &vecWidth, int &vecHeight) override {};
    void updateVelocity();
    void resetVelocity();
    int getBlocksToFall();
    double randomNumber();
    void swapElements(std::vector<std::vector<Pixel *>> &vec, int x1, int y1, int x2, int y2);
    void setProcessed(bool tf) override;
    bool getProcessed() override;

private:
    int velocity{0};
};


#endif /* SOLIDDYNAMIC_HPP */
