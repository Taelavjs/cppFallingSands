#include "Liquid.hpp"

#include "SolidDynamic.hpp"

Liquid::Liquid() {
    setIsMoveable(true);
    setIsLiquid(true);
}
Liquid::~Liquid() {}

int Liquid::getMovingDirection(){
    return x_direction;
}

void Liquid::moveHorizontally(int &vecWidth, Chunk &vec, int col, int row, int incrementor)
{
    int newCol = col + incrementor;
    while (newCol >= 0 && newCol < vecWidth && abs(newCol - col) < xMaxDistance  && (vec[row][newCol] == nullptr|| vec[row][newCol] -> getIsLiquid()))
    {
        newCol += incrementor;
    }
    newCol -= incrementor; // Step back to the last valid position
    swapElements(vec, row, col, row, newCol);
}

void Liquid::update(Chunk &vec, int &row, int &col, int &vecWidth, int &vecHeight, Chunk& leftChunk, Chunk& rightChunk, Chunk& belowChunk, Chunk& aboveChunk)
{
    setProcessed(true);
    bool colLeftInBounds = col - 1 >= 0;
    bool colRightInBounds = col + 1 < vecWidth;
    bool dropInBounds = row + 1 < vecHeight;
    int pRow = row;
    int pCol = col;

    colLeftInBounds = pCol - 1 >= 0;
    colRightInBounds = pCol + 1 < vecWidth;
    dropInBounds = pRow + 1 < vecHeight;
    x_direction = rand() % 2 == 0 ? -1 : 1;
        
    if(x_direction == 0){
        x_direction = rand() % 2 == 0 ? -1 : 1; // Randomize direction
    }

    if(!dropInBounds) {
        Chunk chunk;
        if(belowChunk.size() != 0){
            belowChunk[0][col] = vec[row][col];
            vec[row][col] = nullptr;
            return;
        } 
    }
    
    if (dropInBounds && (vec[row + 1][col] == nullptr || (vec[row+1][col]->getIsLiquid() && vec[row][col]->getDensity() > vec[row+1][col]->getDensity())))
    {

        int blocksToFall{0};
        int fallCounter{1};

        updateVelocity(blocksToFall, 1);

        while(fallCounter < blocksToFall && row+fallCounter < vecHeight && vec[row+fallCounter][col] == nullptr){
            fallCounter++;
        }
        fallCounter--;
        int fallToRow = row + fallCounter;

        swapElements(vec, row, col, fallToRow, col);
        pRow = fallToRow;
        pCol = col;
    } else if((colLeftInBounds && (vec[pRow][pCol-1] == nullptr|| vec[pRow][pCol-1]->getIsLiquid())) && (colRightInBounds && (vec[pRow][pCol+1] == nullptr|| vec[pRow][pCol+1]->getIsLiquid()))){
        moveHorizontally(vecWidth, vec, pCol, pRow, x_direction);
        pCol += x_direction;

    } else if(colRightInBounds && (vec[pRow][pCol+1] == nullptr || vec[pRow][pCol+1]->getIsLiquid())){
        moveHorizontally(vecWidth, vec, pCol, pRow, 1);
        x_direction = 1;
        pCol += 1;

    } else if(colLeftInBounds && (vec[pRow][pCol-1] == nullptr || vec[pRow][pCol-1]->getIsLiquid())){
        moveHorizontally(vecWidth, vec, pCol, pRow, -1);
        x_direction = -1;
        pCol -= 1;
    }

    if(pRow-1 >= 0 && (vec[pRow - 1][pCol] != nullptr && vec[pRow-1][pCol]->getIsMoveable() && !vec[pRow-1][pCol] -> getIsGas() && (vec[pRow][pCol]->getDensity() > vec[pRow-1][pCol]->getDensity()))){
        swapElements(vec, pRow, pCol, pRow-1, pCol);
    }
}
