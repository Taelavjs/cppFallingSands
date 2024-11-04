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
void Gas::update(Chunk &vec, int &row, int &col, int &vecWidth, int &vecHeight, WorldGeneration &worldGeneration)
{
    vec[row][col]->setProcessed(true);
    if(((std::rand() % 2) * 2 - 1 )> 1) return;

    int pCol = col;
    int pRow = row;
    if (randomDir)
        randomDir = rand() % 2 == 0 ? 1 : -1;
    if (isValidPosition(vec, pRow - 1, col))
    {
        // Air space above gas particle
        int newRow = pRow - 1;
        int spaceToRise{};
        updateYPosition(spaceToRise);
        while (std::abs(pRow - newRow) < spaceToRise && newRow >= 0 && (isSpaceFree(vec, newRow, pCol) && !(vec[newRow][pCol] != nullptr && vec[newRow][pCol] -> getIsGas())))
        {
            newRow--;
        }
        newRow++;
        // swapElements(vec, pRow, pCol, newRow, pCol);
        pRow = newRow;
    }

    bool canMoveLeft = (pCol - 1 >= 0 && (isSpaceFree(vec, pRow, pCol-1)));
    bool canMoveRight = (pCol + 1 < vecWidth && (isSpaceFree(vec, pRow, pCol + 1)));

    if (canMoveLeft && canMoveRight) {
        int incr = (std::rand() % 2) * 2 - 1;
        int newCol = pCol + incr;
        while (std::abs(pCol - newCol) < xDispersion && newCol >= 0 && newCol < vecWidth && isValidPosition(vec, pRow, newCol)) {
            newCol += incr;
        }
        newCol -= incr;
        // swapElements(vec, pRow, pCol, pRow, newCol);
    } else if (canMoveLeft) {
        int newCol = pCol - 1;
        while (std::abs(pCol - newCol) < xDispersion && newCol >= 0 && isValidPosition(vec, pRow, newCol)) {
            newCol--;
        }
        newCol++;
        // swapElements(vec, pRow, pCol, pRow, newCol);
    } else if (canMoveRight) {
        int newCol = pCol + 1;
        while (std::abs(pCol - newCol) < xDispersion && newCol < vecWidth && isValidPosition(vec, pRow, newCol)) {
            newCol++;
        }
        newCol--;
        // swapElements(vec, pRow, pCol, pRow, newCol);
    }
}

int Gas::getBlocksToFall()
{
    return -1;
}
