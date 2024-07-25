#include "Gas.hpp"

#include "Gas.hpp"

Gas::Gas()
{
    // Constructor implementation
}

Gas::~Gas()
{
    // Destructor implementation
}

bool Gas::isGas()
{
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
        if (vec[row - 1][newCol] == nullptr)
        {
            res = newCol; // Update result to the new free position
        }
        else
        {
            break; // Stop if we hit a non-empty space
        }
    }
}
// Function to check if a position is valid for gas movement
bool Gas::isValidPosition(const std::vector<std::vector<Pixel*>>& vec, int row, int col)
{
    return (row >= 0 && row < vec.size() && col >= 0 && col < vec[0].size() &&
            (vec[row][col] == nullptr || (!vec[row][col]->isGas() && vec[row][col]->isMoveable())));
}
void Gas::update(std::vector<std::vector<Pixel *>> &vec, int &row, int &col, int &vecWidth, int &vecHeight)
{
    vec[row][col]->setProcessed(true);

    int xDispersion{20}; // Distance to check for dispersion
    int res{col};       // Default to current column
    
    if (isValidPosition(vec, row - 1, col))
    {
        // Air space above gas particle
        int newRow = row - 1;
        while (row - newRow < xDispersion && newRow >= 0 && (vec[newRow][col] == nullptr || (!vec[newRow][col]->isGas() && vec[newRow][col]->isMoveable())))
        {
            newRow--;
        }
        newRow++;
        swapElements(vec, row, col, newRow, col);
    }
    else if (isValidPosition(vec, row - 1, col) && vec[row - 1][col]->isMoveable())
    {
        if (isValidPosition(vec, row - 1, col - 1))
        {
            int newCol = col - 1;
            while (col - newCol < xDispersion && newCol >= 0 && (vec[row-1][newCol] == nullptr || (!vec[row-1][newCol]->isGas() && vec[row-1][newCol]->isMoveable())))
            {
                newCol--;
            }
            newCol++;
            swapElements(vec, row, col, row - 1, newCol);
        }
        else if (isValidPosition(vec, row - 1, col + 1))
        {
            int newCol = col + 1;
            while (newCol - col < xDispersion && newCol < vecWidth &&(vec[row-1][newCol] == nullptr || (!vec[row-1][newCol]->isGas() && vec[row-1][newCol]->isMoveable())))
            {
                newCol++;
            }
            newCol--;
            swapElements(vec, row, col, row - 1, newCol);
        }
    }
    bool canMoveLeft = (col - 1 >= 0 && (vec[row][col - 1] == nullptr || !vec[row][col-1]->isGas()));
    bool canMoveRight = (col + 1 < vecWidth && (vec[row][col + 1] == nullptr || !vec[row][col+1]->isGas()));

    if (canMoveLeft && canMoveRight){
        const int randomBit = rand() % 2;
        int incr;
        if(randomBit == 0){
            incr = 1;
        } else {
            incr = -1;
        }
    
        int newCol = col + incr;
        while (std::abs(col - newCol) < 20 && newCol >= 0 && newCol < vecWidth && isValidPosition(vec, row, newCol))
        {
            newCol += incr;
        }
        newCol -= incr;
        swapElements(vec, row, col, row, newCol);
    } else if(canMoveLeft)
    {
        int newCol = col - 1;
        while (col - newCol < 20 && newCol >= 0 && isValidPosition(vec, row, newCol))
        {
            newCol--;
        }
        newCol++;
        swapElements(vec, row, col, row, newCol);
    }
    else if (canMoveRight)
    {
        int newCol = col + 1;
        while (newCol - col < 20 && newCol < vecWidth && isValidPosition(vec, row, newCol))
        {
            newCol++;
        }
        newCol--;
        swapElements(vec, row, col, row, newCol);
    }
}

int Gas::getBlocksToFall()
{
    int acceleration = g / mass;
    yVelocity += acceleration;
    int blocksToFall = std::max(1, std::min(25, yVelocity / 2)); // Divide by a higher value to reduce the falling speed
    return -1 * blocksToFall;
}
