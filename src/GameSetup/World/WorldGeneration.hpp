#ifndef WORLDGENERATION
#define WORLDGENERATION
#include "../../Utility/ProceduralTerrainGen.hpp"
#include "../../Utility/Velocity.hpp"
#include <iostream>
#include <iomanip> // Required for std::setprecision

#include <vector>
#include "../../Elements/BaseElements/Pixel.hpp"
#include "../../Elements/Napalm.hpp"
#include "../../Elements/Oil.hpp"
#include "../../Elements/Rock.hpp"
#include "../../Elements/Sand.hpp"
#include "../../Elements/Smoke.hpp"
#include "../../Elements/Water.hpp"
#include <map>
#include "Chunk.hpp"

class Chunk;
class Napalm;
class Oil;
class Smoke;
class Rock;
class Water;
class Sand;
class WorldGeneration {
public:
    WorldGeneration(int newWidth);
    ~WorldGeneration();
    void cleanUp();
    void generateBlock();
    void pixelsToBlocks(std::vector<float> noise, Vector2D worldQuad, Chunk &chunk);
    void generateCorridors(std::vector<float> noise, Vector2D worldQuad, Chunk &chunk);
    Chunk& getLocalVec();
    Vector2D getGlobalCoordinates(Vector2D position);
    std::map<Vector2D, Chunk>& getVecStore();
    Vector2D getGlobalCoordinates(int chunkX, int chunkY, int localX, int localY, int chunkSizeX, int chunkSizeY);
    Chunk& getChunk(Vector2D chunkGlobalCoord);
private:
    std::map<Vector2D, Chunk> worldVecStore;
    Sand *sand;
    Water *water;
    Rock *rock;
    Smoke *smoke;
    Oil *oil;
    Napalm *napalm;
    int width{0};
    Chunk emptyChunk;
};

#endif /* WORLDGENERATION */
