#include "Water.hpp"

Water::Water()
{
    movingRight = (randomNumber() > 0.5f);
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
        // Check if you can fall left or right diagonally
        bool canFallRight = col + 1 < vecWidth && row< vecHeight && vec[row][col + 1] == nullptr;
        bool canFallLeft = col - 1 >= 0 && row < vecHeight && vec[row][col - 1] == nullptr;

        // Check if you can move left or right on the same row
        bool canMoveRight = col + 1 < vecWidth && vec[row][col + 1] == nullptr;
        bool canMoveLeft = col - 1 >= 0 && vec[row][col - 1] == nullptr;

        resetVelocity();
        if (canFallRight || canFallLeft)
        {
            if (canFallRight && !canFallLeft)
            {
                swapElements(vec, row, col, row, col + 1);
                movingRight = true;
            }
            else if (canFallLeft && !canFallRight)
            {
                swapElements(vec, row, col, row, col - 1);
                movingRight = false;
            }
            else if (canFallLeft && canFallRight)
            {
                swapElements(vec, row, col, row, col + x_direction);
            }
        }
        else if (canMoveRight || canMoveLeft)
        {
            // Prefer diagonal movement over horizontal movement
            if (canMoveRight && !canMoveLeft)
            {
                swapElements(vec, row, col, row, col + 1);
                movingRight = true;
            }
            else if (canMoveLeft && !canMoveRight)
            {
                swapElements(vec, row, col, row, col - 1);
                movingRight = false;
            }
            else if (canMoveLeft && canMoveRight)
            {
                swapElements(vec, row, col, row, col + x_direction);
            }
        }
        // If neither direction is possible, do nothing (water stays in place)
    }
}
