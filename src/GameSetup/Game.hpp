#ifndef GAME_HPP
#define GAME_HPP

#include <vector>
#include <thread>
#include <random>
#include <string>
#include <iostream>
#include <SDL2/SDL.h>
#include <future>
#include "../Elements/BaseElements/Pixel.hpp"
#include "../Elements/Sand.hpp"
#include "../Elements/Water.hpp"
#include "../Elements/Rock.hpp"
#include "../Elements/Smoke.hpp"
#include "../Elements/Oil.hpp"
#include "../Elements/Napalm.hpp"
#include "Rendering.hpp"
#include "../Playables/Player.hpp"
#include "../Textures/Sprite.hpp"
#include "World/WorldGeneration.hpp"
#include "../Utility/ThreadPool.hpp"

class Game {
public:
    Game(int vecWidth, int vecHeight);
    ~Game();

    void init(const std::string* title, int scaleX, int scaleY);
    void handleEvents(const uint8_t &xScale, const uint8_t &yScale);
    void update( const int& xScale, const int& yScale);
    void render();
    void clean();
    void setRunning(bool running) { isRunning = running; }
    bool getRunning() const { return isRunning; }
    void swapElements(int x, int y, int x2, int y2);
    double randomnumber();
    void updateSequence(int &vecWidth, int &vecHeight, int &row, int &col, Chunk &vec);
    void chunkUpdates(int chunkStart, int chunkEnd);
    void pixelsToBlocks(std::vector<float> pixels);
    void ChunkUpdateSkipping(int startingChunkRow, int startingChunkCol, int numChunksY, int numChunksX, int chunkSizeY, int chunkSizeX, int vecHeight, int vecWidth, Chunk &vec);
    void worker(int startingChunkRow, int startingChunkCol, int numChunksY, int numChunksX, int chunkSizeY, int chunkSizeX, int vecHeight, int vecWidth, Chunk &vec, int rowChunk, int colChunk);
    void generateCorridors(std::vector<float> pixels);

private:
    int vecWidth;
    int vecHeight;
    // int rendererScalex;
    // int rendererScaley;
    bool isRunning;
    WorldGeneration worldGeneration;
    Player* player;
    Uint8* prevKeys = nullptr;
    int numKeys = 0;
    Sand *sand;
    Water *water;
    Rock *rock;
    Smoke *smoke;
    Oil *oil;
    Napalm *napalm;
    ThreadPool threads;
};

#endif // GAME_HPP
