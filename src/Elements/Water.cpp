#include "Water.hpp"

Water::Water()
{
    movingRight = (randomNumber() > 0.5f);
    density = 2;
}

Water::~Water() {}

void Water::update(std::vector<std::vector<Pixel *>> &vec, int &row, int &col, int &vecWidth, int &vecHeight)
{
    int x_direction = rand() % 2 == 0 ? -1 : 1; // Randomize direction

    if (row + 1 < vecHeight && vec[row + 1][col] == nullptr)
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
    else
    {
        if (row + 1 < vecHeight && col + 1 < vecWidth && vec[row + 1][col + 1] == nullptr && col - 1 >= 0 && vec[row + 1][col - 1] == nullptr)
        {
            // Could fall either left diagonally or right diagonally
            updateVelocity();
            swapElements(vec, row, col, row, col + x_direction);
        }
        else if (row + 1 < vecHeight && col + 1 < vecWidth && vec[row + 1][col + 1] == nullptr)
        {
            // Could fall right diagonally
            updateVelocity();
            swapElements(vec, row, col, row + 1, col + 1);
        }
        else if (row + 1 < vecHeight && col - 1 >= 0 && vec[row + 1][col - 1] == nullptr)
        {
            // Could fall left diagonally
            updateVelocity();
            swapElements(vec, row, col, row + 1, col - 1);
        }
        else if (col - 1 >= 0 && vec[row][col - 1] == nullptr && col + 1 < vecWidth && vec[row][col + 1] == nullptr)
        {
            // Has to move to the left or right
            int newCol = col + x_direction;
            while (newCol >= 0 && newCol < vecWidth && vec[row][newCol] == nullptr && abs(newCol - col) < getDispersionRate())
            {
                newCol += x_direction;
            }
            newCol -= x_direction;
            updateVelocity();
            swapElements(vec, row, col, row, newCol);
        }
        else if (col + 1 < vecWidth && vec[row][col + 1] == nullptr)
        {
            // Has to move to the right
            int newCol = col + 1;
            while (newCol < vecWidth && vec[row][newCol] == nullptr && newCol < col + getDispersionRate())
            {
                newCol++;
            }
            newCol--; // Step back to the last valid position
            updateVelocity();
            swapElements(vec, row, col, row, newCol);
        }
        else if (col - 1 >= 0 && vec[row][col - 1] == nullptr)
        {
            // Has to move to the left
            int newCol = col - 1;
            while (newCol >= 0 && vec[row][newCol] == nullptr && newCol > col - getDispersionRate())
            {
                newCol--;
            }
            newCol++; // Step back to the last valid position
            updateVelocity();
            swapElements(vec, row, col, row, newCol);
        }
        // If neither direction is possible, do nothing (water stays in place)
    }
}

