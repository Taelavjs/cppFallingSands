#include "WorldGeneration.hpp"

WorldGeneration::WorldGeneration(int newWidth)
:width(newWidth), emptyChunk()
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
    // Clear the map itself
    worldVecStore.clear();
}

void WorldGeneration::generateBlock() {
    std::vector<std::vector<Pixel *>> vec1(width, std::vector<Pixel *>(width));
    std::vector<std::vector<Pixel *>> vec2(width, std::vector<Pixel *>(width));
    std::vector<std::vector<Pixel *>> vec3(width, std::vector<Pixel *>(width));
    std::vector<std::vector<Pixel *>> vec4(width, std::vector<Pixel *>(width));
    worldVecStore[Vector2D(0,0)] = Chunk(Vector2D(0, 0), vec1);
    worldVecStore[Vector2D(0, 1)] = Chunk(Vector2D(0, 1), vec2);
    worldVecStore[Vector2D(1, 0)] = Chunk(Vector2D(1, 0), vec3);
    worldVecStore[Vector2D(1, 1)] = Chunk(Vector2D(1, 1), vec4);

    for (auto& mapEntry : worldVecStore) {
        Chunk& chunk = mapEntry.second;
        std::vector<float> pixels(width * width);
        pixels = ProceduralTerrainGen::createNoise(width, width);
        pixelsToBlocks(pixels, mapEntry.first, chunk);
        pixels = ProceduralTerrainGen::createTerrain(width, width);
        std::cout << "Pixels Length : " << pixels.size()/(width) << '\n';
        generateCorridors(pixels, mapEntry.first, chunk);
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

void WorldGeneration::pixelsToBlocks(std::vector<float> noise, Vector2D worldQuad, Chunk &vec)
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
                vec[row][col] = rock->clone();
                total += 1;
            } 
        }
    }
}

double getRandomDouble(double min, double max) {
    // Create a random device and a random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    
    // Define the distribution range
    std::uniform_real_distribution<> dis(min, max);
    
    // Generate and return a random number
    return dis(gen);
}

void WorldGeneration::generateCorridors(std::vector<float> noise, Vector2D worldQuad, Chunk &vec)
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

    for (int row = 0; row < width; ++row)
    {
        for (int col = 0; col < width; ++col)
        {
            if(vec[row][col] != nullptr){
                continue;
            }
            if (getRandomDouble(0, 1) < 0.2)
            {
                vec[row][col] = water->clone();
            } else if(getRandomDouble(0, 1) < 0.2){
                vec[row][col] = oil->clone();
            }
        }
    }
}

Chunk& WorldGeneration::getLocalVec(){
    return worldVecStore[Vector2D(0, 0)];
}

std::map<Vector2D, Chunk>& WorldGeneration::getVecStore(){
    return worldVecStore;
}

Vector2D WorldGeneration::getGlobalCoordinates(Vector2D position){
    Vector2D result(0, 0);
    result.x = std::floor(position.x / width);
    result.y = std::floor(position.y / width);

    std::cout << "Global Coordinates : " << result.x << " " << result.y << '\n';
    return result;
}

Vector2D WorldGeneration::getGlobalCoordinates(int chunkX, int chunkY, int localX, int localY, int chunkSizeX, int chunkSizeY) {
    return Vector2D(chunkX * width + localX, chunkY * width + localY);
}

Chunk& WorldGeneration::getChunk(Vector2D chunkGlobalCoord){
    if(worldVecStore.find(chunkGlobalCoord) != worldVecStore.end()){
        return worldVecStore[chunkGlobalCoord];;
    } 
    return emptyChunk;
}
