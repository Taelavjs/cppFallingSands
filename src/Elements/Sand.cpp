#include "Sand.hpp"

Sand::Sand(){
    density = 5;
}
Sand::~Sand(){}

bool checkMoveableMat(Pixel* space){
    return (space->isLiquid() || space->isGas());
}

void Sand::xDisperse(std::vector<std::vector<Pixel *>> &vec, int row, int col, int xDispersion, int xDirection, int &res)
{
    // Initialize result with the current position
    res = col;

    for (int i = 1; i <= xDispersion; i++)
    {
        int newCol = col + (xDirection * i);
        if (newCol < 0 || newCol >= vec[0].size())
        {
            break; // Out of bounds
        }
        if (vec[row + 1][newCol] == nullptr || checkMoveableMat(vec[row+1][newCol]))
        {
            res = newCol; // Update result to the new free position
        }
        else
        {
            break; // Stop if we hit a non-empty space
        }
    }
}


void Sand::update(std::vector<std::vector<Pixel *>> &vec, int &row, int &col, int &vecWidth, int &vecHeight) 
{
    vec[row][col]->setProcessed(true);

    int xDispersion{5}; // Distance to check for dispersion
    int res{col}; // Default to current column

    // Check space below
    if (row + 1 < vecHeight && vec[row + 1][col] == nullptr) 
    {
        // Space directly below is free
        int blocksToFall = getBlocksToFall();
        int fallToRow = row + blocksToFall;
        if (fallToRow >= vecHeight) 
        {
            fallToRow = vecHeight - 1;
        }
        while (fallToRow > row && vec[fallToRow][col] != nullptr) 
        {
            fallToRow--;
        }
        if (fallToRow > row) 
        {
            updateVelocity();
            swapElements(vec, row, col, fallToRow, col);
        }
    } 
    else if (row+1 < vecHeight && vec[row+1][col] != nullptr && checkMoveableMat(vec[row+1][col])){
        swapElements(vec, row, col, row+1, col);
        resetVelocity();
    } else {
        // Space directly below is not free, check spaces to the sides
        bool canFallLeft = (col - 1 >= 0 && row + 1 < vecHeight && (vec[row + 1][col - 1] == nullptr || vec[row+1][col-1]->isLiquid()));
        bool canFallRight = (col + 1 < vecWidth && row + 1 < vecHeight && (vec[row + 1][col + 1] == nullptr || vec[row+1][col+1]->isLiquid()));

        if (canFallLeft && canFallRight) 
        {
            // Randomly choose between falling left or right if both are free
            double rngValue = randomNumber();
            if (rngValue > 0.5f) 
            {
                xDisperse(vec, row, col, xDispersion, -1, res);
            } 
            else 
            {
                xDisperse(vec, row, col, xDispersion, 1, res);
            }
        } 
        else if (canFallLeft) 
        {
            xDisperse(vec, row, col, xDispersion, -1, res);
        } 
        else if (canFallRight) 
        {
            xDisperse(vec, row, col, xDispersion, 1, res);
        }
        
        // Move sand to the calculated position
        if (res != col)
        {
            resetVelocity();
            swapElements(vec, row, col, row + 1, res);
        }
    }
}



