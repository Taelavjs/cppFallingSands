#include "WorldGeneration.hpp"

Sand* WorldGeneration::sand = new Sand();
Water* WorldGeneration::water = new Water();
Rock* WorldGeneration::rock = new Rock();
Smoke* WorldGeneration::smoke = new Smoke();
Napalm* WorldGeneration::napalm = new Napalm();
Oil* WorldGeneration::oil = new Oil();
int WorldGeneration::width{0};

std::vector<std::vector<Pixel *>>  WorldGeneration::vec(WorldGeneration::width, 
                                                        std::vector<Pixel *>(WorldGeneration::width));

void WorldGeneration::generateBlock(int newWidth) {
    WorldGeneration::width = newWidth;
    WorldGeneration::vec.resize(newWidth);
    for (int i = 0; i < newWidth; ++i) {
        WorldGeneration::vec[i].resize(newWidth);
    }
    
    std::vector<float> pixels(newWidth * newWidth);
    pixels = ProceduralTerrainGen::createNoise(newWidth, newWidth);
    WorldGeneration::pixelsToBlocks(pixels);
    pixels = ProceduralTerrainGen::createTerrain(newWidth, newWidth);
    WorldGeneration::generateCorridors(pixels);
}


void WorldGeneration::cleanUp(){
    delete WorldGeneration::sand;
    delete WorldGeneration::water;
    delete WorldGeneration::rock;
    delete WorldGeneration::oil;
    delete WorldGeneration::smoke;
    delete WorldGeneration::napalm;
}

void WorldGeneration::pixelsToBlocks(std::vector<float> noise)
{
    for (int row = 0; row < WorldGeneration::width; ++row)
    {
        for (int col = 0; col < WorldGeneration::width; ++col)
        {
            const float pixValue = noise[row * width + col];
            if (pixValue > -0.2f)
            {
                vec[row][col] = WorldGeneration::rock->clone();
            }
        }
    }
}

void WorldGeneration::generateCorridors(std::vector<float> noise)
{
    for (int row = 0; row < WorldGeneration::width; ++row)
    {
        for (int col = 0; col < WorldGeneration::width; ++col)
        {
            const float pixValue = noise[row * WorldGeneration::width + col];
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
