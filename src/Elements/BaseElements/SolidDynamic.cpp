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

void SolidDynamic::update(int &row, int &col, int &vecWidth, int &vecHeight, WorldGeneration &worldGeneration)
{
    setProcessed(true);

    bool moved = true;
    int blocksFallen = 0;
    while (moved && blocksFallen <= 4) {
        moved = false; // Assume no movement; change if a swap is made

        Pixel* pixBelow = worldGeneration.getPixelFromGlobal(Vector2D(col, row + 1));
        if (row + 1 < 384 && pixBelow == nullptr) {
            // Move down if the space below is empty
            worldGeneration.swapTwoValues(Vector2D(col, row), Vector2D(col, row + 1));
            row += 1; // Update row after moving
            x_direction = 0;
            moved = true; // Indicate a move was made
            blocksFallen++;
        } else if (row + 1 < 384 && pixBelow != nullptr && pixBelow->getIsLiquid()) {
            // Move down if the space below has a less dense liquid
            worldGeneration.swapTwoValues(Vector2D(col, row), Vector2D(col, row + 1));
            row += 1; // Update row after moving
            x_direction = 0;
            moved = true; // Indicate a move was made
            blocksFallen++;
        } else {
            // If no valid move down is possible, break the loop
            break;
        }
    }

    // No lateral movement checks; pixel can only move left or right in a separate update
    if(blocksFallen > 0) {
        yVelocity += 2;
        yVelocity = std::min(yVelocity, 6);
        return;
    };
    yVelocity = 1;

    blocksFallen = 0;
    moved = true;
    while (moved && blocksFallen <= xVelocity) {
        moved = false; // Assume no movement; change if a swap is made

        Pixel* leftDownPix = worldGeneration.getPixelFromGlobal(Vector2D(col - 1, row + 1));
        Pixel* rightDownPix = worldGeneration.getPixelFromGlobal(Vector2D(col + 1, row + 1));
        Pixel* leftPix = worldGeneration.getPixelFromGlobal(Vector2D(col - 1, row));
        Pixel* rightPix = worldGeneration.getPixelFromGlobal(Vector2D(col + 1, row));
        if(row+1 >= 384) break;
        bool isLeftValid = col - 1 >= 0 && (leftDownPix == nullptr || (leftDownPix->getIsLiquid())) && (leftPix == nullptr || (leftPix->getIsLiquid()));
        bool isRightValid = col + 1 < 384 && ( rightDownPix == nullptr || (rightDownPix->getIsLiquid())) && ( rightPix == nullptr || (rightPix->getIsLiquid()));

        if (isLeftValid && isRightValid) {
            // Randomly decide to move left or right if both directions are valid
            x_direction = x_direction == 0 ? (rand() % 2 == 0 ? -1 : 1) : x_direction;
            worldGeneration.swapTwoValues(Vector2D(col, row), Vector2D(col + x_direction, row));
            col += x_direction; // Update column after moving
            moved = true; // Indicate a move was made
            blocksFallen++;
        } else if (isLeftValid) {
            // Move left if only left is valid
            worldGeneration.swapTwoValues(Vector2D(col, row), Vector2D(col - 1, row));
            col -= 1; // Update column after moving
            x_direction = -1;
            moved = true; // Indicate a move was made
            blocksFallen++;
        } else if (isRightValid) {
            // Move right if only right is valid
            worldGeneration.swapTwoValues(Vector2D(col, row), Vector2D(col + 1, row));
            col += 1; // Update column after moving
            x_direction = 1;
            moved = true; // Indicate a move was made
            blocksFallen++;
        } else {
            // No valid horizontal move found, break the loop
            break;
        }
    }

    if(blocksFallen > 0) {
        xVelocity += 1;
        xVelocity = std::min(xVelocity, 2);
        return;
    };

    xVelocity = 1;



    // int xDispersion{5}; // Distance to check for dispersion
    // int res{col};       // Default to current column

    // // Check space below
    // if (row + 1 < vecHeight && isSpaceFree(vec, row+1, col))
    // {
    //     int newRow = row + 1;
    //     int blocksToFall{};
    //     updateVelocity(blocksToFall, 1);
    //     while (newRow < vecHeight && isSpaceFree(vec, newRow, col) && (newRow - row) < blocksToFall)
    //     {
    //         newRow++;
    //     }
    //     newRow--; // Step back to the last valid position
    //     if (newRow != row) // Only swap if new position is different
    //     {
    //         // swapElements(vec, row, col, newRow, col);
    //     }
    // }
    // else if (row + 1 < vecHeight && vec[row + 1][col] != nullptr && checkMoveableMat(vec[row + 1][col]))
    // {
    //     // Space directly below is free
    //     int newRow = row + 1;
    //     int blocksToFall{};
    //     updateVelocity(blocksToFall, -1);
    //     while (newRow < vecHeight && vec[newRow][col] != nullptr && (isSpaceFree(vec, newRow, col)) && (newRow - row) < blocksToFall)
    //     {
    //         newRow++;
    //     }
    //     newRow--; // Step back to the last valid position
    //     if (newRow != row) // Only swap if new position is different
    //     {
    //         // swapElements(vec, row, col, newRow, col);
    //     }
    // }
    // else
    // {
    //     // Space directly below is not free, check spaces to the sides
    //     bool canFallLeft = (col - 1 >= 0 && row + 1 < vecHeight && (isSpaceFree(vec, row+1, col-1)));
    //     bool canFallRight = (col + 1 < vecWidth && row + 1 < vecHeight && (isSpaceFree(vec, row+1, col+1)));

    //     if (canFallLeft && canFallRight)
    //     {
    //         // Randomly choose between falling left or right if both are free
    //         double rngValue = randomNumber();
    //         if (rngValue > 0.5f)
    //         {
    //             xDisperse(vec, row, col, xDispersion, -1, res);
    //         }
    //         else
    //         {
    //             xDisperse(vec, row, col, xDispersion, 1, res);
    //         }
    //     }
    //     else if (canFallLeft)
    //     {
    //         xDisperse(vec, row, col, xDispersion, -1, res);
    //     }
    //     else if (canFallRight)
    //     {
    //         xDisperse(vec, row, col, xDispersion, 1, res);
    //     }

    //     // Move sand to the calculated position
    //     if (res != col)
    //     {
    //         // swapElements(vec, row, col, row + 1, res);
    //     }
    //     resetVelocity();

    // }
}
