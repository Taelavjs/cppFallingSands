#include "WorldGeneration.hpp"
#include "../../Elements/BaseElements/Pixel.hpp"
#include "../../Elements/Napalm.hpp"
#include "../../Elements/Oil.hpp"
#include "../../Elements/Rock.hpp"
#include "../../Elements/Sand.hpp"
#include "../../Elements/Smoke.hpp"
#include "../../Elements/Water.hpp"

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
    std::vector<std::vector<Pixel *>> vec(width, std::vector<Pixel *>(width));
    worldVecStore[Vector2D(0,0)] = Chunk(Vector2D(0, 0), vec);
    worldVecStore[Vector2D(0, 1)] = Chunk(Vector2D(0, 1), vec);
    worldVecStore[Vector2D(1, 0)] = Chunk(Vector2D(1, 0), vec);
    worldVecStore[Vector2D(1, 1)] = Chunk(Vector2D(1, 1), vec);

    std::vector<float> noiseMap = ProceduralTerrainGen::createNoise(width, width);
    std::vector<float> terrainMap = ProceduralTerrainGen::createTerrain(width, width);  

    for (auto& mapEntry : worldVecStore) {
        Chunk& chunk = mapEntry.second;
        pixelsToBlocks(noiseMap, mapEntry.first, chunk);
        generateCorridors(terrainMap, mapEntry.first, chunk);
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
            int globalRow = chunkStartY + row;
            int globalCol = chunkStartX + col;
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
            int globalRow = chunkStartY + row;
            int globalCol = chunkStartX + col;
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
            if (getRandomDouble(0, 1) < 0.8)
            {
                vec[row][col] = water->clone();
            } else if(getRandomDouble(0, 1) < 0.4){
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
        return worldVecStore[chunkGlobalCoord];
    } 
    return emptyChunk;
}

// Make the class able to give the correct chunk using global coordinates
// Rather than swapping between global and local, much easier

Pixel* WorldGeneration::getPixelFromGlobal(Vector2D position){
    width = 192;
    Vector2D chunkCoord(0, 0);
    Vector2D localCoord(0, 0);
    
    chunkCoord.x = std::floor(position.x / width);
    chunkCoord.y = std::floor(position.y / width);

    localCoord.x = fmod(position.x, width);
    if (localCoord.x < 0) localCoord.x += width; // Handle negative coordinates

    localCoord.y = fmod(position.y, width);
    if (localCoord.y < 0) localCoord.y += width; // Handle negative coordinates

    if(worldVecStore.find(chunkCoord) != worldVecStore.end()){
        return worldVecStore[chunkCoord][localCoord.y][localCoord.x]; // row x col
    } 
    return nullptr;
}

void WorldGeneration::clearPixelProcessed(){
    for (auto& mapEntry : worldVecStore) {
        Chunk& vec2D = mapEntry.second;
        Vector2D globalCoords = mapEntry.first;

        for(int i = 0; i < vec2D.size(); i++){
            for(int j = 0; j < vec2D[i].size(); j++){
                Pixel* pix = vec2D[i][j];
                if(pix != nullptr) {
                    pix->setProcessed(false);
                }

            }
        }
    }
}

void WorldGeneration::swapTwoValues(Vector2D pos1, Vector2D pos2){
    Vector2D chunkCoord(0, 0);
    Vector2D localCoord(0, 0);
    
    chunkCoord.x = std::floor(pos1.x / width);
    chunkCoord.y = std::floor(pos1.y / width);

    localCoord.x = fmod(pos1.x, width);
    if (localCoord.x < 0) localCoord.x += width; // Handle negative coordinates

    localCoord.y = fmod(pos1.y, width);
    if (localCoord.y < 0) localCoord.y += width; // Handle negative coordinates

    Vector2D chunkCoord2(0, 0);
    Vector2D localCoord2(0, 0);
    
    chunkCoord2.x = std::floor(pos2.x / width);
    chunkCoord2.y = std::floor(pos2.y / width);

    localCoord2.x = fmod(pos2.x, width);
    if (localCoord2.x < 0) localCoord2.x += width; // Handle negative coordinates

    localCoord2.y = fmod(pos2.y, width);
    if (localCoord2.y < 0) localCoord2.y += width; // Handle negative coordinates

    Chunk& ch1 = worldVecStore[chunkCoord];
    Chunk& ch2 = worldVecStore[chunkCoord2];
   if(ch1.size() == 0 || ch2.size()==0){
        return;
    }
    if(ch1[localCoord.y][localCoord.x] != nullptr ) {
        ch1[localCoord.y][localCoord.x]->setProcessed(true);
    }
    if(ch2[localCoord2.y][localCoord2.x] != nullptr ) {
        ch2[localCoord2.y][localCoord2.x] ->setProcessed(true);
    }
    Pixel* temp = ch1[localCoord.y][localCoord.x];
    ch1[localCoord.y][localCoord.x] = ch2[localCoord2.y][localCoord2.x];
    ch2[localCoord2.y][localCoord2.x] = temp;
}
