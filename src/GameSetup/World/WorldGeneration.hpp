#ifndef WORLDGENERATION
#define WORLDGENERATION
#include "../../Utility/ProceduralTerrainGen.hpp"
#include "../../Utility/Velocity.hpp"
#include <vector>
#include "../../Elements/BaseElements/Pixel.hpp"
#include "../../Elements/Napalm.hpp"
#include "../../Elements/Oil.hpp"
#include "../../Elements/Rock.hpp"
#include "../../Elements/Sand.hpp"
#include "../../Elements/Smoke.hpp"
#include "../../Elements/Water.hpp"
#include <map>
class WorldGeneration {
public:
    WorldGeneration(int newWidth);
    ~WorldGeneration();
    void cleanUp();
    void generateBlock();
    void pixelsToBlocks(std::vector<float> noise, Vector2D worldQuad, std::vector<std::vector<Pixel *>> &vec);
    void generateCorridors(std::vector<float> noise, Vector2D worldQuad, std::vector<std::vector<Pixel *>> &vec);
    std::vector<std::vector<Pixel *>>& getLocalVec();
    Vector2D getGlobalCoordinates(Vector2D position);
private:
    std::map<Vector2D, std::vector<std::vector<Pixel *>>> worldVecStore;
    Sand *sand;
    Water *water;
    Rock *rock;
    Smoke *smoke;
    Oil *oil;
    Napalm *napalm;
    int width{0};
};

#endif /* WORLDGENERATION */
