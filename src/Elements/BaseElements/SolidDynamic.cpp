#include "SolidDynamic.hpp"

SolidDynamic::SolidDynamic() {
    moveable = true;

}
SolidDynamic::~SolidDynamic() {}

void SolidDynamic::resetVelocity()
{
    yVelocity = 0;
}

void SolidDynamic::updateVelocity(int &newCol)
{
    newCol += (int)2.0f/getMass() + yVelocity;
    yVelocity += (int)2.0f;
    if(yVelocity > terminalY) yVelocity = terminalY;

}

double SolidDynamic::randomNumber()
{
    static std::default_random_engine rng(static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count()));

    // Create a uniform distribution between 0.0 and 1.0
    static std::uniform_real_distribution<double> dist(0.0, 1.0);

    // Return a random double value in the range [0.0, 1.0)
    return dist(rng);
}


void SolidDynamic::setProcessed(bool tf)
{
    isProcessed = tf;
}

bool SolidDynamic::getProcessed()
{
    return isProcessed;
}



void SolidDynamic::xDisperse(std::vector<std::vector<Pixel *>> &vec, int row, int col, int xDispersion, int xDirection, int &res)
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
    return (space->isLiquid() || space->isGas());
}


void SolidDynamic::update(std::vector<std::vector<Pixel *>> &vec, int &row, int &col, int &vecWidth, int &vecHeight)
{
    vec[row][col]->setProcessed(true);

    int xDispersion{5}; // Distance to check for dispersion
    int res{col};       // Default to current column

    // Check space below
    if (row + 1 < vecHeight && vec[row + 1][col] == nullptr)
    {
        int newRow = row + 1;
        int blocksToFall{};
        updateVelocity(blocksToFall);
        while (newRow < vecHeight && vec[newRow][col] == nullptr && (newRow - row) < blocksToFall)
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
        updateVelocity(blocksToFall);
        while (newRow < vecHeight && vec[newRow][col] != nullptr && vec[newRow][col]->isLiquid() && (newRow - row) < blocksToFall)
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
        bool canFallLeft = (col - 1 >= 0 && row + 1 < vecHeight && (vec[row + 1][col - 1] == nullptr || vec[row + 1][col - 1]->isLiquid()));
        bool canFallRight = (col + 1 < vecWidth && row + 1 < vecHeight && (vec[row + 1][col + 1] == nullptr || vec[row + 1][col + 1]->isLiquid()));

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
