#include "Game.hpp"
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
#include "../Utility/GlobalVariables.hpp"

Game::Game()
    : isRunning(true), worldGeneration(), threads()
{
    sand = new Sand();
    water = new Water();
    rock = new Rock();
    smoke = new Smoke();
    oil = new Oil();
    napalm = new Napalm();
    SDL_GetKeyboardState(&numKeys);
    prevKeys = new Uint8[numKeys];
    SDL_PumpEvents();
    numChunksX = GlobalVariables::screenSize / chunkSizeX;
    numChunksY = GlobalVariables::screenSize / chunkSizeY;

}

Game::~Game()
{
    // SDL_DestroyRenderer(renderer);
    // SDL_DestroyWindow(window);
    // SDL_Quit();
    delete sand;
    delete water;
    delete rock;
    delete oil;
    delete smoke;
    delete napalm;
}

void Game::init()
{
    worldGeneration.generateBlock();
    char *playerSpritePath{"Sprites/AnimationSheet_Character.png"};

    Rendering::setValues();
    int width{32};
    int height{32};
    int rows{9};
    int cols{8};
    Sprite *playerSprite = new Sprite(playerSpritePath, width, height, rows, cols);
    player = new Player(playerSprite);
    
}

void SquarePlace(Chunk &vec, int x, int y, Pixel *elm)
{
    int numToPlace{1};
    for (int j = x - numToPlace; j < x + numToPlace; j++)
    {
        for (int k = y - numToPlace; k < y + numToPlace; k++)
        {
            if (j < 0 || k < 0 || k >= vec.size() || j >= vec[k].size())
                continue;
            // if(rand() % 2 == 0) continue;

            delete vec[k][j];         // Delete existing object to avoid memory leak
            vec[k][j] = elm->clone(); // Assign new clone
        }
    }
}

void Game::handleEvents()
{
    const Uint8 *e = SDL_GetKeyboardState(&numKeys);
    Chunk localChunk = worldGeneration.getLocalVec();

    int x{}, y{};
    if (e)
    {
        SDL_GetMouseState(&x, &y);
        if (e[SDL_SCANCODE_P])
        {
            setRunning(false);
        }
        else if (e[SDL_SCANCODE_A])
        {
            SquarePlace(localChunk, x / GlobalVariables::rendererScale, y / GlobalVariables::rendererScale, water);
        }
        else if (e[SDL_SCANCODE_D])
        {
            SquarePlace(localChunk, x / GlobalVariables::rendererScale, y / GlobalVariables::rendererScale, rock);
        }
        else if (e[SDL_SCANCODE_S])
        {
            SquarePlace(localChunk, x / GlobalVariables::rendererScale, y / GlobalVariables::rendererScale, sand);
        }
        else if (e[SDL_SCANCODE_W])
        {
            SquarePlace(localChunk, x / GlobalVariables::rendererScale, y / GlobalVariables::rendererScale, smoke);
        }
        else if (e[SDL_SCANCODE_Q])
        {
            SquarePlace(localChunk, x / GlobalVariables::rendererScale, y / GlobalVariables::rendererScale, oil);
        }
        else if (e[SDL_SCANCODE_F])
        {
            SquarePlace(localChunk, x / GlobalVariables::rendererScale, y / GlobalVariables::rendererScale, napalm);
        }

        for(int i = 0; i < numKeys; ++i){
            if(e[i] == 1){
                player->playerInputHandler((SDL_Scancode)i);
            }
        }
    }
    for(int i = 0; i < numKeys; ++i){
        if(prevKeys[i] == 1 && e[i] == 0){
            player -> playerReleaseHandler((SDL_Scancode)i);
        }
    }
    memcpy(prevKeys, e, numKeys);
    SDL_PumpEvents();
}

void Game::updateSequence(int &row, int &col)
{
    Pixel* pixel = worldGeneration.getPixelFromGlobal(Vector2D(col, row));
    if (pixel == nullptr)
        return;
    if (pixel->getProcessed())
        return;
    if (pixel->getIsFlammable())
    {
        if (worldGeneration.getPixelFromGlobal(Vector2D(col, row))->fireTick(worldGeneration, row, col))
        {
            worldGeneration.burntSmoke(row, col);
            return;
        };
    }
    pixel->update(row, col, GlobalVariables::screenSize, GlobalVariables::screenSize, worldGeneration);
} 

void Game::worker(Vector2D globalChunk, int startingChunkRow, int startingChunkCol)
{
    // Calculate the boundaries of the current chunk


    int chunkOffsetX = globalChunk.x * GlobalVariables::screenSize;
    int chunkOffsetY = globalChunk.y * GlobalVariables::screenSize;
    int rowStart = chunkOffsetX + (startingChunkRow * chunkSizeX);
    int colStart = chunkOffsetY + (startingChunkCol * chunkSizeY);
    int rowEnd = std::min(rowStart + chunkSizeY,  GlobalVariables::screenSize * 2); 
    int colEnd = std::min(colStart + chunkSizeX, GlobalVariables::screenSize * 2);
    // Update the 8x8 chunk
    for (int row = rowStart; row < rowEnd; ++row)
    {
        for (int col = colStart; col < colEnd; ++col)
        {
            updateSequence(row, col);
        }
    }
}

void Game::ChunkUpdateSkipping(Vector2D& globalChunk, int startingChunkRow, int startingChunkCol)
{
    std::vector<std::future<void>> futures;

    // Enqueue tasks
    for (int rowChunk = startingChunkRow; rowChunk < numChunksY; rowChunk += 2)
    {
        for (int colChunk = startingChunkCol; colChunk < numChunksX; colChunk += 2)
        {
            auto future = threads.enqueue([this, globalChunk, rowChunk, colChunk]()
            {
                this->worker(globalChunk, rowChunk, colChunk);
            });
            futures.push_back(std::move(future));  // Move future into vector
        }
    }
    for (auto& future : futures)
    {
        future.get();  // This will wait for each task to complete
    }
}

void Game::update()
{
    Chunk& vec = worldGeneration.getChunk(worldGeneration.getGlobalCoordinates(player->getCoordinates()));

    Vector2D dimensions = player->getDimensions();
    std::map<Vector2D, Chunk>& chunks = worldGeneration.getVecStore();
    // Runs over each chunk, specifying what sub chunk the X and Y update sequence should start at
    // It allows for no two chunks directly next to each other to be updating at the same time, allowing for multicore processing
    // Without dirty reads/rights

    // Current issues with fire ticks and check for getIsFlammable - Unsure why
    for (auto& mapEntry : chunks) {
        Chunk& vec2D = mapEntry.second;
        Vector2D globalCoords = mapEntry.first;
        ChunkUpdateSkipping(globalCoords, 1, 1);
        ChunkUpdateSkipping(globalCoords, 1, 0);
        ChunkUpdateSkipping(globalCoords, 0, 1);
        ChunkUpdateSkipping(globalCoords, 0, 0);
    }
    player->update(vec, Rendering::getRenderer(), GlobalVariables::screenSize);
    worldGeneration.clearPixelProcessed();

}

void Game::render()
{
    std::map<Vector2D, Chunk>& temp = worldGeneration.getVecStore();
    for (auto& mapEntry : temp) {
        Chunk& vec2D = mapEntry.second;
        Vector2D globalCoords = mapEntry.first;
        Rendering::renderGrid(vec2D, player, globalCoords);
    }
    Rendering::renderPlayer(player);
    Rendering::showRendering();
}

void Game::clean() {
};

double Game::randomnumber()
{
    static std::default_random_engine rng;
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    return dist(rng);
}
