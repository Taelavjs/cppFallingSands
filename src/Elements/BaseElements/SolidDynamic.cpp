#include "SolidDynamic.hpp"

SolidDynamic::SolidDynamic() {}
SolidDynamic::~SolidDynamic() {}

void SolidDynamic::resetVelocity()
{
    transferVelocityX();
    yVelocity = 0;
}

void SolidDynamic::updateVelocity()
{
    // Constants for simulation
    const float drag = 0.98f;         // Drag factor (0 < drag < 1)
    const float maxVelocity = 8000.0f;  // Terminal yVelocity

    // Update the yVelocity with gravity
    yVelocity += g / 3;

    // Apply drag to the yVelocity
    yVelocity *= drag;
    xVelocity *= drag;

    // Ensure yVelocity does not exceed terminal yVelocity
    if (yVelocity > maxVelocity)
    {
        yVelocity = maxVelocity;
    }
}

void SolidDynamic::transferVelocityX(){
    xVelocity += std::max(minDispersionRate, std::min(static_cast<int>(verticalToHorizontalRation * yVelocity), maxDispersionRate));
    
}

int SolidDynamic::getBlocksToFall()
{
    int acceleration = g / mass;
    yVelocity += acceleration;
    int blocksToFall = std::max(1, std::min(25, yVelocity / 2)); // Divide by a higher value to reduce the falling speed
    return blocksToFall;
}

double SolidDynamic::randomNumber()
{
    static std::default_random_engine rng(static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count()));

    // Create a uniform distribution between 0.0 and 1.0
    static std::uniform_real_distribution<double> dist(0.0, 1.0);

    // Return a random double value in the range [0.0, 1.0)
    return dist(rng);
}

void SolidDynamic::swapElements(std::vector<std::vector<Pixel*>>& vec,
                                int x1, int y1,
                                int x2, int y2)
{


    if (x1 != x2) {
        // Swap elements in different rows
        Pixel* temp = vec[x1][y1];
        vec[x1][y1] = vec[x2][y2];
        vec[x2][y2] = temp;
        return;
    }

    // Swap elements horizontally within the same row
    if (y1 > y2) {
        for (int y = y2; y < y1; ++y) {
            Pixel* temp = vec[x1][y];
            vec[x1][y] = vec[x1][y+1];
            vec[x1][y+1] = temp;
        }
    } else {
        for (int y = y2; y > y1; --y) {
            Pixel* temp = vec[x1][y];
            vec[x1][y] = vec[x1][y-1];
            vec[x1][y-1] = temp;
        }
    }
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
        while (newRow < vecHeight && vec[newRow][col] == nullptr && (newRow - row) < getBlocksToFall())
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
        while (newRow < vecHeight && vec[newRow][col] != nullptr && vec[newRow][col]->isLiquid() && (newRow - row) < getBlocksToFall())
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
