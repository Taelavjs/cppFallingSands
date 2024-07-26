#include "Liquid.hpp"

#include "SolidDynamic.hpp"

Liquid::Liquid() {}
Liquid::~Liquid() {}

bool Liquid::isLiquid(){
    return true;
}

double Liquid::randomNumber()
{
    static std::default_random_engine rng(static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count()));
    static std::uniform_real_distribution<double> dist(0.0, 1.0);
    return dist(rng);
}

void Liquid::setProcessed(bool tf)
{
    isProcessed = tf;
}

bool Liquid::getProcessed()
{
    return isProcessed;
}

void Liquid::moveHorizontally(int &vecWidth, std::vector<std::vector<Pixel *>> &vec, int col, int row, int incrementor)
{
    int newCol = col + incrementor;
    while (newCol >= 0 && newCol < vecWidth && vec[row][newCol] == nullptr && abs(newCol - col) < xMaxDistance)
    {
        newCol += incrementor;
    }
    newCol -= incrementor; // Step back to the last valid position
    swapElements(vec, row, col, row, newCol);
}

void Liquid::update(std::vector<std::vector<Pixel *>> &vec, int &row, int &col, int &vecWidth, int &vecHeight)
{
    bool colLeftInBounds = col - 1 >= 0;
    bool colRightInBounds = col + 1 < vecWidth;
    bool dropInBounds = row + 1 < vecHeight;
    int pRow = row;
    int pCol = col;
        
    if(x_direction == 0){
        x_direction = rand() % 2 == 0 ? -1 : 1; // Randomize direction
    }
    


    if (row + 1 < vecHeight && (vec[row + 1][col] == nullptr))
    {
        int blocksToFall = 3;
        int fallCounter{1};

        while(fallCounter < blocksToFall && row+fallCounter < vecHeight && vec[row+fallCounter][col] == nullptr){
            fallCounter++;
        }
        fallCounter--;
        int fallToRow = row + fallCounter;

        swapElements(vec, row, col, fallToRow, col);
        pRow = fallToRow;
        pCol = col;
    }

    colLeftInBounds = pCol - 1 >= 0;
    colRightInBounds = pCol + 1 < vecWidth;
    dropInBounds = pRow + 1 < vecHeight;
    x_direction = rand() % 2 == 0 ? -1 : 1;

    if((colLeftInBounds && vec[pRow][pCol-1] == nullptr) && (colRightInBounds && vec[pRow][pCol+1] == nullptr)){
        moveHorizontally(vecWidth, vec, pCol, pRow, x_direction);

    } else if(colLeftInBounds && vec[pRow][pCol+1] == nullptr){
        moveHorizontally(vecWidth, vec, pCol, pRow, 1);
        x_direction = 1;

    } else if(colRightInBounds && vec[pRow][pCol-1] == nullptr){
        moveHorizontally(vecWidth, vec, pCol, pRow, -1);
        x_direction = -1;
    } else{
    }


}
