#ifndef SOLIDDYNAMIC_HPP
#define SOLIDDYNAMIC_HPP
#include "Pixel.hpp"
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <random>
#include "Moveable.hpp"
class SolidDynamic : public Moveable {
public:

protected:    
    SolidDynamic();
    virtual ~SolidDynamic();
protected:
    virtual void update(std::vector<std::vector<Pixel *>> &vec, int &row, int &col, int &vecWidth, int &vecHeight);
    virtual void xDisperse(std::vector<std::vector<Pixel *>> &vec, int row, int col, int xDispersion, int xDirection, int &res);
    bool checkMoveableMat(Pixel *space);
};


#endif /* SOLIDDYNAMIC_HPP */
