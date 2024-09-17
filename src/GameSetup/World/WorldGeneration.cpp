#include "WorldGeneration.hpp"

WorldGeneration::WorldGeneration(int newWidth)
:width(newWidth)
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
    for (const auto& mapEntry : worldVecStore) {
        const std::vector<std::vector<Pixel *>>& vec2D = mapEntry.second;
        
        for (const auto& row : vec2D) {
            for (Pixel* pixelPtr : row) {
                if (pixelPtr != nullptr) {
                    delete pixelPtr;
                }
            }
        }
    }

    // Clear the map itself
    worldVecStore.clear();
}

void WorldGeneration::generateBlock() {
    std::vector<std::vector<Pixel *>> vec1(width, std::vector<Pixel *>(width));
    std::vector<std::vector<Pixel *>> vec2(width, std::vector<Pixel *>(width));
    std::vector<std::vector<Pixel *>> vec3(width, std::vector<Pixel *>(width));
    std::vector<std::vector<Pixel *>> vec4(width, std::vector<Pixel *>(width));
    worldVecStore[Vector2D(0,0)] = vec1;
    worldVecStore[Vector2D(0, 1)] = vec2;
    worldVecStore[Vector2D(1, 0)] = vec3;
    worldVecStore[Vector2D(1, 1)] = vec4;

    for (auto& mapEntry : worldVecStore) {
        std::vector<std::vector<Pixel *>>& vec2D = mapEntry.second;
        std::vector<float> pixels(width * width);
        pixels = ProceduralTerrainGen::createNoise(width, width);
        pixelsToBlocks(pixels, mapEntry.first, vec2D);
        pixels = ProceduralTerrainGen::createTerrain(width, width);
        std::cout << "Pixels Length : " << pixels.size()/(width) << '\n';
        generateCorridors(pixels, mapEntry.first, vec2D);
    }
}


void WorldGeneration::cleanUp(){
    delete sand;
    delete water;
    delete rock;
    delete oil;
    delete smoke;
    delete napalm;
}

void printVector(const std::vector<float>& vec) {
    std::cout << "Vector elements:\n";
    std::cout << std::fixed << std::setprecision(2);
    for (const auto& element : vec) {
        std::cout << element << " ";
    }
    std::cout << std::endl;
}

void WorldGeneration::pixelsToBlocks(std::vector<float> noise, Vector2D worldQuad, std::vector<std::vector<Pixel *>> &vec)
{
    int chunkStartX = worldQuad.x * width;
    int chunkStartY = worldQuad.y * width;
    std::cout << worldQuad.x << worldQuad.y << '\n';
    int total = 0;
    std::setprecision(2);
    for (int row = 0; row < width; ++row)
    {
        for (int col = 0; col < width; ++col)
        {
            int globalRow = chunkStartX + row;
            int globalCol = chunkStartY + col;
            const float pixValue = noise[(globalRow) * (width * 2) + (globalCol)];
            if (pixValue > -0.2f)
            {
                vec[row][col] = water->clone();
                total += 1;
            } 
        }
    }
    std::cout << "total : " << total << '\n';
}

void WorldGeneration::generateCorridors(std::vector<float> noise, Vector2D worldQuad, std::vector<std::vector<Pixel *>> &vec)
{
    int chunkStartX = worldQuad.x * width;
    int chunkStartY = worldQuad.y * width;
    for (int row = 0; row < width; ++row)
    {
        for (int col = 0; col < width; ++col)
        {
            int globalRow = chunkStartX + row;
            int globalCol = chunkStartY + col;
            const float pixValue = noise[(globalRow) * (width*2) + (globalCol)];            
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
    return worldVecStore[Vector2D(0, 0)];
}

std::map<Vector2D, std::vector<std::vector<Pixel *>>>& WorldGeneration::getVecStore(){
    return worldVecStore;
}

Vector2D WorldGeneration::getGlobalCoordinates(Vector2D position){
    Vector2D result(0, 0);
    result.x = std::floor(position.x / width);
    result.y = std::floor(position.y / width);

    std::cout << "Global Coordinates : " << result.x << " " << result.y << '\n';
    return result;
}
