#include "Gas.hpp"

#include "Gas.hpp"

Gas::Gas() {
    // Constructor implementation
}

Gas::~Gas() {
    // Destructor implementation
}

bool Gas::isGas(){
    return true;
    };

void Gas::xDisp(std::vector<std::vector<Pixel *>> &vec, int row, int col, int xDispersion, int xDirection, int &res)
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
        if (vec[row-1][newCol] == nullptr)
        {
            res = newCol; // Update result to the new free position
        }
        else
        {
            break; // Stop if we hit a non-empty space
        }
    }
}

void Gas::update(std::vector<std::vector<Pixel *>> &vec, int &row, int &col, int &vecWidth, int &vecHeight)
{
    vec[row][col]->setProcessed(true);

    int xDispersion{5}; // Distance to check for dispersion
    int res{col};       // Default to current column

    // Check space below
    if (row - 1 >= 0 && vec[row - 1][col] == nullptr)
    {
        int newRow = row - 1;
        while (newRow >= 0 && vec[newRow][col] == nullptr && (newRow + row) > getBlocksToFall())
        {
            newRow--;
        }
        newRow++; // Step back to the last valid position
        if (newRow != row) // Only swap if new position is different
        {
            swapElements(vec, row, col, newRow, col);
        }
    }
    else if (row - 1 >= 0 && vec[row - 1][col] != nullptr && (vec[row-1][col]->isSolid() || vec[row-1][col]->isLiquid()))
    {
        // Space directly below is free
        int newRow = row - 1;
        while (newRow >= 0 && (vec[row-1][col]->isSolid() || vec[row-1][col]->isLiquid()))
        {
            newRow--;
        }
        newRow++; // Step back to the last valid position
        if (newRow != row) // Only swap if new position is different
        {
            swapElements(vec, row, col, newRow, col);
        }
    }
    else
    {
    bool canFallLeft = (col - 1 >= 0 && row-1 >= 0 && vec[row-1][col - 1] == nullptr);
    bool canFallRight = (col + 1 < vecWidth && row-1 >= 0 && vec[row-1][col + 1] == nullptr);

        if (canFallLeft && canFallRight)
        {
            // Randomly choose between falling left or right if both are free
            double rngValue = randomNumber();
            if (rngValue > 0.5f)
            {
                xDisp(vec, row, col, 20, -1, res);
            }
            else
            {
                xDisp(vec, row, col, 20, 1, res);
            }
        }
        else if (canFallLeft)
        {
            xDisp(vec, row, col, 20, -1, res);
        }
        else if (canFallRight)
        {
            xDisp(vec, row, col, 20, 1, res);
        }

        // Move sand to the calculated position
        if (res != col)
        {
            swapElements(vec, row, col, row, res);
        }
        resetVelocity();

    }
}

int Gas::getBlocksToFall()
{
    int acceleration = g / mass;
    yVelocity += acceleration;
    int blocksToFall = std::max(1, std::min(25, yVelocity / 2)); // Divide by a higher value to reduce the falling speed
    return -1 * blocksToFall;
}
