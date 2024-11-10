#include "Gas.hpp"

#include "Gas.hpp"

Gas::Gas()
{
    // Constructor implementation
    setMass(300);
    setIsMoveable(true);
    setIsGas(true);
}

Gas::~Gas()
{
    // Destructor implementation
}

bool Gas::isGas()
{
    return true;
};

void Gas::updateYPosition(int &newCol){
    newCol -= (int)1.0f/getMass() + yVelocity;
    yVelocity -= (int)1.0f;
    if(yVelocity < (-1 * getTerminalY())) yVelocity = (-1 * getTerminalY());
}

void Gas::xDisp(Chunk &vec, int row, int col, int xDispersion, int xDirection, int &res)
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
        if (isSpaceFree(vec, row-1, newCol))
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
bool Gas::isValidPosition(Chunk &vec, int row, int col)
{
    return (row >= 0 && row < vec.size() && col >= 0 && col < vec[0].size() && isSpaceFree(vec, row, col));
}
void Gas::update(int row, int col, int &vecWidth, int &vecHeight, WorldGeneration &worldGeneration)
{
    setProcessed(true);

    bool moved = true;
    int blocksFallen = 0;
    while (moved && blocksFallen <= yVelocity) {
        moved = false; 

        Pixel*& pixBelow = worldGeneration.getPixelFromGlobal(Vector2D(col, row - 1));
        if (row - 1 >= 0 && pixBelow == nullptr) {
            worldGeneration.swapTwoValues(Vector2D(col, row), Vector2D(col, row - 1));
            row -= 1;
            x_direction = 0;
            moved = true; 
            blocksFallen++;
        } else if (row - 1 >= 0 && pixBelow != nullptr && (pixBelow->getIsMoveable()) && !pixBelow->getIsGas()) {
            worldGeneration.swapTwoValues(Vector2D(col, row), Vector2D(col, row - 1));
            row -= 1; // Update row after moving
            x_direction = 0;
            moved = true; // Indicate a move was made
            blocksFallen++;
        } else {
            // If no valid move down is possible, break the loop
            break;
        }
    }

    if(blocksFallen > 0) {
        yVelocity += 2;
        yVelocity = std::min(yVelocity, 6);
        return;
    };
    yVelocity = 1;

    blocksFallen = 0;
    moved = true;
    while (moved && blocksFallen <= 4) {
        moved = false; 

        Pixel* leftPix = worldGeneration.getPixelFromGlobal(Vector2D(col - 1, row));
        Pixel* rightPix = worldGeneration.getPixelFromGlobal(Vector2D(col + 1, row));

        bool isLeftValid = col - 1 >= 0 && (leftPix == nullptr || (leftPix->getIsMoveable()) && !leftPix->getIsGas());
        bool isRightValid = col + 1 < 384 && ( rightPix == nullptr || (rightPix->getIsMoveable()) && !rightPix->getIsGas());

        if (isLeftValid && isRightValid) {
            x_direction = x_direction == 0 ? (rand() % 2 == 0 ? -1 : 1) : x_direction;
            worldGeneration.swapTwoValues(Vector2D(col, row), Vector2D(col + x_direction, row));
            col += x_direction; 
            moved = true; 
            blocksFallen++;
        } else if (isLeftValid) {
            worldGeneration.swapTwoValues(Vector2D(col, row), Vector2D(col - 1, row));
            col -= 1; 
            x_direction = -1;
            moved = true; 
            blocksFallen++;
        } else if (isRightValid) {
            worldGeneration.swapTwoValues(Vector2D(col, row), Vector2D(col + 1, row));
            col += 1; 
            x_direction = 1;
            moved = true;
            blocksFallen++;
        } else {
            break;
        }
    }

    if(blocksFallen > 0) {
        return;
    };

}

int Gas::getBlocksToFall()
{
    return -1;
}
