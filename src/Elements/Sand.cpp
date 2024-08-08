#include "Sand.hpp"

Sand::Sand()
{
    setDensity(5);
    setMass(2);
}
Sand::~Sand() {}

// bool checkMoveableMat(Pixel *space)
// {
//     return (space->isLiquid() || space->isGas());
// }


// void Sand::update(std::vector<std::vector<Pixel *>> &vec, int &row, int &col, int &vecWidth, int &vecHeight)
// {
//     vec[row][col]->setProcessed(true);

//     int xDispersion{5}; // Distance to check for dispersion
//     int res{col};       // Default to current column

//     // Check space below
//     if (row + 1 < vecHeight && vec[row + 1][col] == nullptr)
//     {
//         int newRow = row + 1;
//         while (newRow < vecHeight && vec[newRow][col] == nullptr && (newRow - row) < getBlocksToFall())
//         {
//             newRow++;
//         }
//         newRow--; // Step back to the last valid position
//         if (newRow != row) // Only swap if new position is different
//         {
//             swapElements(vec, row, col, newRow, col);
//         }
//     }
//     else if (row + 1 < vecHeight && vec[row + 1][col] != nullptr && checkMoveableMat(vec[row + 1][col]))
//     {
//         // Space directly below is free
//         int newRow = row + 1;
//         while (newRow < vecHeight && vec[newRow][col] != nullptr && vec[newRow][col]->isLiquid() && (newRow - row) < getBlocksToFall())
//         {
//             newRow++;
//         }
//         newRow--; // Step back to the last valid position
//         if (newRow != row) // Only swap if new position is different
//         {
//             swapElements(vec, row, col, newRow, col);
//         }
//     }
//     else
//     {
//         // Space directly below is not free, check spaces to the sides
//         bool canFallLeft = (col - 1 >= 0 && row + 1 < vecHeight && (vec[row + 1][col - 1] == nullptr || vec[row + 1][col - 1]->isLiquid()));
//         bool canFallRight = (col + 1 < vecWidth && row + 1 < vecHeight && (vec[row + 1][col + 1] == nullptr || vec[row + 1][col + 1]->isLiquid()));

//         if (canFallLeft && canFallRight)
//         {
//             // Randomly choose between falling left or right if both are free
//             double rngValue = randomNumber();
//             if (rngValue > 0.5f)
//             {
//                 xDisperse(vec, row, col, xDispersion, -1, res);
//             }
//             else
//             {
//                 xDisperse(vec, row, col, xDispersion, 1, res);
//             }
//         }
//         else if (canFallLeft)
//         {
//             xDisperse(vec, row, col, xDispersion, -1, res);
//         }
//         else if (canFallRight)
//         {
//             xDisperse(vec, row, col, xDispersion, 1, res);
//         }

//         // Move sand to the calculated position
//         if (res != col)
//         {
//             swapElements(vec, row, col, row + 1, res);
//         }
//         resetVelocity();

//     }
// }
