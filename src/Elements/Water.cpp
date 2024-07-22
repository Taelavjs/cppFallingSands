#include "Water.hpp"

Water::Water(){
    movingRight = (randomNumber() > 0.5f) ? true : false;
}

Water::~Water(){}

void Water::update(std::vector<std::vector<Pixel *>> &vec, int &row, int &col, int &vecWidth, int &vecHeight) {
    if(row+1 < vecHeight && vec[row+1][col] == nullptr){
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
    } else {
        // Check if you can fall left or right
        bool canFallRight = col + 1 < vecWidth && row + 1 < vecHeight && vec[row + 1][col + 1] == nullptr;
        bool canFallLeft = col - 1 >= 0 && row + 1 < vecHeight && vec[row + 1][col - 1] == nullptr;

        // Check if you can move left or right on the same row
        bool canMoveRight = col + 1 < vecWidth && vec[row][col + 1] == nullptr;
        bool canMoveLeft = col - 1 >= 0 && vec[row][col - 1] == nullptr;

        resetVelocity();


        if(canFallRight || canFallLeft){
            if(canFallRight && !canFallLeft){
                swapElements(vec, row, col, row + 1, col + 1);
                movingRight = true;
            } else if(canFallLeft && !canFallRight){
                swapElements(vec, row, col, row+1, col - 1);
                movingRight = false;
            } else if(canFallLeft && canFallRight) {
                int k = (movingRight) ? 1 : -1;
                swapElements(vec, row, col, row+1, col + k);
            }
            
        } else if (canMoveRight && canMoveLeft) {
            // Both directions possible, move in the current direction

            int k = (movingRight) ? 1 : -1;
            swapElements(vec, row, col, row, col + k);
        } else if (canMoveRight) {
            // Only move right
            swapElements(vec, row, col, row, col + 1);
            movingRight = true;
        } else if (canMoveLeft) {
            // Only move left
            swapElements(vec, row, col, row, col - 1);
            movingRight = false;
        }
        // If neither direction is possible, do nothing (water stays in place)
    }
}
