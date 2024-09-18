#ifndef SOLIDDYNAMIC_HPP
#define SOLIDDYNAMIC_HPP
#include "Pixel.hpp"
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <random>
#include "Moveable.hpp"
class WorldGeneration;

class SolidDynamic : public Moveable {
public:

protected:    
    SolidDynamic();
    virtual ~SolidDynamic();
protected:
    virtual void update(Chunk &vec, int &row, int &col, int &vecWidth, int &vecHeight, Chunk& leftChunk, Chunk& rightChunk, Chunk& belowChunk, Chunk& aboveChunk);
    virtual void xDisperse(Chunk &vec, int row, int col, int xDispersion, int xDirection, int &res);
    bool checkMoveableMat(Pixel *space);
};


#endif /* SOLIDDYNAMIC_HPP */
