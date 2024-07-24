#include "Water.hpp"

Water::Water()
{
    movingRight = (randomNumber() > 0.5f);
    density = 2;
}

Water::~Water() {}

void Water::moveHorizontally(int &vecWidth, std::vector<std::vector<Pixel *>> &vec, int col, int row, int incrementor)
{
    int newCol = col + incrementor;
    transferVelocityX();
    resetVelocity();
    while (newCol >= 0 && newCol < vecWidth && vec[row][newCol] == nullptr && abs(newCol - col) < getDispersionRate())
    {
        newCol += incrementor;
    }
    newCol -= incrementor; // Step back to the last valid position
    swapElements(vec, row, col, row, newCol);
}

uint32_t Water::getColour() {
    int k = std::max(1, std::min(getSumVelocity(), 10));
    double k_adjusted = std::pow(k, 0.7); // Adjust exponent as needed
    int blue_value = std::min(255, static_cast<int>(120 + k_adjusted * 100)); // Increase max blue and adjust scaling
    return SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32), 0, 0, blue_value, 200);
}

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

        resetX();
    }
    else
    {
        bool colLeftInBounds = col - 1 >= 0;
        bool colRightInBounds = col + 1 < vecWidth;
        bool dropInBounds = row + 1 < vecHeight;

        if (dropInBounds && col + 1 < vecWidth && vec[row + 1][col + 1] == nullptr && col - 1 >= 0 && vec[row + 1][col - 1] == nullptr)
        {
            // Could fall either left diagonally or right diagonally
            updateVelocity();
            swapElements(vec, row, col, row, col + x_direction);
            resetX();

        }
        else if (dropInBounds && col + 1 < vecWidth && vec[row + 1][col + 1] == nullptr)
        {
            // Could fall right diagonally
            updateVelocity();
            swapElements(vec, row, col, row + 1, col + 1);
        resetX();

        }
        else if (dropInBounds && col - 1 >= 0 && vec[row + 1][col - 1] == nullptr)
        {
            // Could fall left diagonally
            updateVelocity();
            swapElements(vec, row, col, row + 1, col - 1);
        resetX();

        }
        else if (colLeftInBounds && vec[row][col - 1] == nullptr && colRightInBounds && vec[row][col + 1] == nullptr)
        {
            // Has to move to the left or right
            moveHorizontally(vecWidth, vec, col, row, x_direction);
        }
        else if (colRightInBounds && vec[row][col + 1] == nullptr)
        {
            moveHorizontally(vecWidth, vec, col, row, 1);
        }
        else if (colLeftInBounds && vec[row][col - 1] == nullptr)
        {
            moveHorizontally(vecWidth, vec, col, row, -1);
            // If neither direction is possible, do nothing (water stays in place)
        }
    }
}
