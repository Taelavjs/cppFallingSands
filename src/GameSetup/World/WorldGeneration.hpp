#ifndef WORLDGENERATION
#define WORLDGENERATION
#include "../../Utility/ProceduralTerrainGen.hpp"
#include <vector>
#include "../../Elements/BaseElements/Pixel.hpp"
#include "../../Elements/Napalm.hpp"
#include "../../Elements/Oil.hpp"
#include "../../Elements/Rock.hpp"
#include "../../Elements/Sand.hpp"
#include "../../Elements/Smoke.hpp"
#include "../../Elements/Water.hpp"
class WorldGeneration {
public:
    WorldGeneration() = delete;
    void cleanUp();
    static void generateBlock(int newWidth);
    static void pixelsToBlocks(std::vector<float> noise);
    static void generateCorridors(std::vector<float> noise);
    static std::vector<std::vector<Pixel *>>& getLocalVec();
private:
    static std::vector<std::vector<Pixel *>> vec;
    static Sand *sand;
    static Water *water;
    static Rock *rock;
    static Smoke *smoke;
    static Oil *oil;
    static Napalm *napalm;
    static int width;
};

#endif /* WORLDGENERATION */
