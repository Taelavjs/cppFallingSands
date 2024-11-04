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
    // int newCol = col + incrementor;
    // while (newCol >= 0 && newCol < vecWidth && abs(newCol - col) < xMaxDistance  && (vec[row][newCol] == nullptr|| vec[row][newCol] -> getIsLiquid()))
    // {
    //     newCol += incrementor;
    // }
    // newCol -= incrementor; // Step back to the last valid position
    // swapElements(vec, row, col, row, newCol);
}

void Liquid::update(int &row, int &col, int &vecWidth, int &vecHeight, WorldGeneration &worldGeneration)
{
    Pixel* pixBelow = worldGeneration.getPixelFromGlobal(Vector2D(col, row + 1)) ;
    if(row+1 < 384 && pixBelow == nullptr)
    {
        worldGeneration.swapTwoValues(Vector2D(col, row), Vector2D(col, row+1));
        x_direction = 0;
    } else if(row+1 < 384 && pixBelow != nullptr && pixBelow->getIsLiquid() && pixBelow->getDensity() < getDensity() ){
        worldGeneration.swapTwoValues(Vector2D(col, row), Vector2D(col, row+1));
        x_direction = 0;
        
    } else {
        Pixel* leftPix = worldGeneration.getPixelFromGlobal(Vector2D(col - 1, row));
        Pixel* rightPix = worldGeneration.getPixelFromGlobal(Vector2D(col + 1, row));

        

        bool isLeftValid = col - 1 >= 0;
        bool isRightValid = col + 1 < 384;

        if(isLeftValid && isRightValid && (leftPix == nullptr || leftPix->getIsLiquid() && leftPix->getDensity() < getDensity())  && (rightPix == nullptr || rightPix->getIsLiquid() && rightPix->getDensity() < getDensity())){
            //5050 move left or right
            if (x_direction == 0)
            {
                x_direction = rand() % 2 == 0 ? -1 : 1; // Randomize direction
            }

            worldGeneration.swapTwoValues(Vector2D(col, row), Vector2D(col + x_direction, row));

        } else if(isLeftValid && (leftPix == nullptr || leftPix->getIsLiquid() && leftPix->getDensity() < getDensity()) ){
            worldGeneration.swapTwoValues(Vector2D(col, row), Vector2D(col - 1, row));
            x_direction = -1;
        } else if(isRightValid && (rightPix == nullptr || rightPix->getIsLiquid() && rightPix->getDensity() < getDensity())){
            worldGeneration.swapTwoValues(Vector2D(col, row), Vector2D(col + 1, row));
            x_direction = 1;
        }
    }

    setProcessed(true);

    // bool colLeftInBounds = col - 1 >= 0;
    // bool colRightInBounds = col + 1 < vecWidth;
    // bool dropInBounds = row + 1 < vecHeight;
    // int pRow = row;
    // int pCol = col;

    // colLeftInBounds = pCol - 1 >= 0;
    // colRightInBounds = pCol + 1 < vecWidth;
    // dropInBounds = pRow + 1 < vecHeight;
        
    // if(x_direction == 0){
    //     x_direction = rand() % 2 == 0 ? -1 : 1; // Randomize direction
    // }

    // if(!dropInBounds && belowChunk.size() != 0 && (belowChunk[0][col] == nullptr || (belowChunk[0][col]->getIsLiquid() && getDensity() < belowChunk[0][col]->getDensity()))) {
    //     Pixel* temp = belowChunk[0][col];
    //     belowChunk[0][col] = vec[row][col];
    //     vec[row][col] = temp;
        
    //     return;
    // } else if(!colRightInBounds && rightChunk.size() != 0 && (rightChunk[pRow][0] == nullptr || rightChunk[pRow][0] ->getIsLiquid())){
    //     x_direction = 1;
    //     pCol += 1;
    //     int moveCounter = 1;
    //     while(moveCounter < 2 && moveCounter < vecHeight && rightChunk[pRow][moveCounter] == nullptr){
    //         moveCounter++;
    //     }
    //     moveCounter--;
    //     Pixel* temp = rightChunk[pRow][moveCounter] ;
    //     rightChunk[pRow][moveCounter]  = vec[pRow][col];
    //     vec[pRow][col] = temp;
        
    //     return;
    // } else if(!colLeftInBounds && leftChunk.size() != 0 && (leftChunk[pRow][vecWidth-1] == nullptr || leftChunk[pRow][vecWidth-1]->getIsLiquid())){
    //     x_direction = -1;
    //     pCol -= 1;
    //     int moveCounter = vecWidth-2;
    //     while(moveCounter > -2 && moveCounter > 0 && leftChunk[pRow][moveCounter] == nullptr){
    //         moveCounter--;
    //     }
    //     moveCounter++;


    //     Pixel* temp = leftChunk[pRow][moveCounter];
    //     leftChunk[pRow][moveCounter]  = vec[pRow][col];
    //     vec[pRow][col] = temp;
        
    //     return;
    // }
    
    // if (dropInBounds && (vec[row + 1][col] == nullptr || (vec[row+1][col]->getIsLiquid() && getDensity() > vec[row+1][col]->getDensity())))
    // {

    //     int blocksToFall{3};
    //     int fallCounter{1};

    //     updateVelocity(blocksToFall, 1);

    //     while(fallCounter < blocksToFall && row+fallCounter < vecHeight && vec[row+fallCounter][col] == nullptr){
    //         fallCounter++;
    //     }
    //     fallCounter--;
    //     int fallToRow = row + fallCounter;

    //     swapElements(vec, row, col, fallToRow, col);
    //     pRow = fallToRow;
    //     pCol = col;
    // } else {
    //     bool isLeftValid{(colLeftInBounds && (vec[pRow][pCol-1] == nullptr|| vec[pRow][pCol-1]->getIsLiquid()))};
    //     bool isRightValid{(colRightInBounds && (vec[pRow][pCol+1] == nullptr|| vec[pRow][pCol+1]->getIsLiquid()))};
    //     if(isLeftValid && isRightValid){
    //         moveHorizontally(vecWidth, vec, pCol, pRow, x_direction);
    //         pCol += x_direction;

    //     } else if(isRightValid){
    //         moveHorizontally(vecWidth, vec, pCol, pRow, 1);
    //         x_direction = 1;
    //         pCol += 1;

    //     } else if(isLeftValid){
    //         moveHorizontally(vecWidth, vec, pCol, pRow, -1);
    //         x_direction = -1;
    //         pCol -= 1;
    //     }
    // }

    // if(pRow-1 >= 0 && (vec[pRow - 1][pCol] != nullptr && vec[pRow-1][pCol]->getIsMoveable() && !vec[pRow-1][pCol] -> getIsGas() && (vec[pRow][pCol]->getDensity() > vec[pRow-1][pCol]->getDensity()))){
    //     swapElements(vec, pRow, pCol, pRow-1, pCol);
    // }
}
