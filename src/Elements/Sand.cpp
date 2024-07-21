#include "Sand.hpp"

Sand::Sand(){
}
Sand::~Sand(){}

void Sand::update(std::vector<std::vector<Pixel *>> &vec, int &row, int &col, int &vecWidth, int &vecHeight) 
{
    if (vec[row + 1][col] != nullptr)
    {
        double rngValue = randomNumber();
        if (col - 1 >= 0 && vec[row + 1][col - 1] == nullptr && rngValue > 0.5f)
        {
            resetVelocity();
            swapElements(vec, row, col, row + 1, col - 1);
        }
        else if (col + 1 < vecWidth && vec[row + 1][col + 1] == nullptr)
        {
            resetVelocity();
            swapElements(vec, row, col, row + 1, col + 1);
        }
    }
    else if (vec[row + 1][col] == nullptr)
    {
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
}
