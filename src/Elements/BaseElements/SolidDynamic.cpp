#include "SolidDynamic.hpp"

SolidDynamic::SolidDynamic() {
    setIsMoveable(true);
    setIsSolid(true);

}
SolidDynamic::~SolidDynamic() {}

void SolidDynamic::xDisperse(Chunk &vec, int row, int col, int xDispersion, int xDirection, int &res)
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
        if (vec[row + 1][newCol] == nullptr || checkMoveableMat(vec[row + 1][newCol]))
        {
            res = newCol; // Update result to the new free position
        }
        else
        {
            break; // Stop if we hit a non-empty space
        }
    }
}

bool SolidDynamic::checkMoveableMat(Pixel *space)
{
    return (space->getIsLiquid() || space->getIsGas());
}

void SolidDynamic::update(Chunk &vec, int &row, int &col, int &vecWidth, int &vecHeight)
{
    setProcessed(true);

    int xDispersion{5}; // Distance to check for dispersion
    int res{col};       // Default to current column

    // Check space below
    if (row + 1 < vecHeight && isSpaceFree(vec, row+1, col))
    {
        int newRow = row + 1;
        int blocksToFall{};
        updateVelocity(blocksToFall, 1);
        while (newRow < vecHeight && isSpaceFree(vec, newRow, col) && (newRow - row) < blocksToFall)
        {
            newRow++;
        }
        newRow--; // Step back to the last valid position
        if (newRow != row) // Only swap if new position is different
        {
            swapElements(vec, row, col, newRow, col);
        }
    }
    else if (row + 1 < vecHeight && vec[row + 1][col] != nullptr && checkMoveableMat(vec[row + 1][col]))
    {
        // Space directly below is free
        int newRow = row + 1;
        int blocksToFall{};
        updateVelocity(blocksToFall, -1);
        while (newRow < vecHeight && vec[newRow][col] != nullptr && (isSpaceFree(vec, newRow, col)) && (newRow - row) < blocksToFall)
        {
            newRow++;
        }
        newRow--; // Step back to the last valid position
        if (newRow != row) // Only swap if new position is different
        {
            swapElements(vec, row, col, newRow, col);
        }
    }
    else
    {
        // Space directly below is not free, check spaces to the sides
        bool canFallLeft = (col - 1 >= 0 && row + 1 < vecHeight && (isSpaceFree(vec, row+1, col-1)));
        bool canFallRight = (col + 1 < vecWidth && row + 1 < vecHeight && (isSpaceFree(vec, row+1, col+1)));

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
            swapElements(vec, row, col, row + 1, res);
        }
        resetVelocity();

    }
}
