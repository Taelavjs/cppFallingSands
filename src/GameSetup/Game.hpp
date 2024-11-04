#ifndef GAME_HPP
#define GAME_HPP

#include <vector>
#include <thread>
#include <random>
#include <string>
#include <iostream>
#include <SDL2/SDL.h>
#include <future>


class Pixel;
class Sand;
class Water;
class Rock;
class Smoke;
class Oil;
class Napalm;
class Rendering;
class Player;
class Sprite;
#include "World/WorldGeneration.hpp"
#include "../Utility/ThreadPool.hpp"
class Chunk;
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
    void updateSequence(int &row, int &col);
    void chunkUpdates(int chunkStart, int chunkEnd);
    void pixelsToBlocks(std::vector<float> pixels);
    void ChunkUpdateSkipping(Vector2D& globalChunk, int startingChunkRow, int startingChunkCol);
    void worker(Vector2D globalChunk, int startingChunkRow, int startingChunkCol);
    void generateCorridors(std::vector<float> pixels);

private:
    int vecWidth;
    int vecHeight;
    // int rendererScalex;
    // int rendererScaley;
    bool isRunning;
    WorldGeneration worldGeneration;
    const int chunkSizeX = 8;
    const int chunkSizeY = 8;
    int numChunksX;
    int numChunksY;
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
