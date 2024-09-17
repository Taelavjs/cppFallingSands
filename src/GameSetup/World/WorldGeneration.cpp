#include "WorldGeneration.hpp"

WorldGeneration::WorldGeneration(int newWidth)
:vec(newWidth, std::vector<Pixel *>(newWidth)), width(newWidth)
{
    sand = new Sand();
    water = new Water();
    rock = new Rock();
    smoke = new Smoke();
    napalm = new Napalm();
    oil = new Oil();

}

WorldGeneration::~WorldGeneration(){
    delete sand;
    delete water;
    delete rock;
    delete oil;
    delete smoke;
    delete napalm;
    for (int i = width - 1; i >= 0; i--)
    {
        for (int k = width - 1; k >= 0; k--)
        {
            if (vec[i][k] == nullptr)
                continue;
            delete vec[i][k];
        }
    }    
}

void WorldGeneration::generateBlock() {
    std::vector<float> pixels(width * width);
    pixels = ProceduralTerrainGen::createNoise(width, width);
    pixelsToBlocks(pixels);
    pixels = ProceduralTerrainGen::createTerrain(width, width);
    generateCorridors(pixels);
}


void WorldGeneration::cleanUp(){
    delete sand;
    delete water;
    delete rock;
    delete oil;
    delete smoke;
    delete napalm;
}

void WorldGeneration::pixelsToBlocks(std::vector<float> noise)
{
    for (int row = 0; row < width; ++row)
    {
        for (int col = 0; col < width; ++col)
        {
            const float pixValue = noise[row * width + col];
            if (pixValue > -0.2f)
            {
                vec[row][col] = rock->clone();
            }
        }
    }
}

void WorldGeneration::generateCorridors(std::vector<float> noise)
{
    for (int row = 0; row < width; ++row)
    {
        for (int col = 0; col < width; ++col)
        {
            const float pixValue = noise[row * width + col];
            if (vec[row][col] != nullptr && vec[row][col]->getIsSolid() && pixValue > 0.6)
            {
                vec[row][col] = nullptr;
            }

            if (row < 30)
            {
                vec[row][col] = nullptr;
            }
        }
    }
}

std::vector<std::vector<Pixel *>>& WorldGeneration::getLocalVec(){
    return WorldGeneration::vec;
}

Vector2D WorldGeneration::getGlobalCoordinates(Vector2D position){
    Vector2D result(0, 0);
    result.x = std::floor(position.x / width);
    result.y = std::floor(position.y / width);

    std::cout << "Global Coordinates : " << result.x << " " << result.y << '\n';
    return result;
}
