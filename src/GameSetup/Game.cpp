#include "Game.hpp"

Game::Game(int vecWidthInp, int vecHeightInp)
    : vecWidth(vecWidthInp - 1), vecHeight(vecHeightInp - 1),
      isRunning(true), worldGeneration(vecWidthInp), threads()
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

void Game::init(const std::string *title, int scaleX, int scaleY)
{
    worldGeneration.generateBlock();
    char *playerSpritePath{"Sprites/AnimationSheet_Character.png"};
    int width{32};
    int height{32};
    int rows{9};
    int cols{8};
    Rendering::setValues(vecWidth, vecHeight, title, scaleX, scaleY);
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

void Game::handleEvents(const uint8_t &xScale, const uint8_t &yScale)
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
            SquarePlace(localChunk, x / xScale, y / yScale, water);
        }
        else if (e[SDL_SCANCODE_D])
        {
            SquarePlace(localChunk, x / xScale, y / yScale, rock);
        }
        else if (e[SDL_SCANCODE_S])
        {
            SquarePlace(localChunk, x / xScale, y / yScale, sand);
        }
        else if (e[SDL_SCANCODE_W])
        {
            SquarePlace(localChunk, x / xScale, y / yScale, smoke);
        }
        else if (e[SDL_SCANCODE_Q])
        {
            SquarePlace(localChunk, x / xScale, y / yScale, oil);
        }
        else if (e[SDL_SCANCODE_F])
        {
            SquarePlace(localChunk, x / xScale, y / yScale, napalm);
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

void Game::chunkUpdates(int chunkStart, int chunkEnd)
{

    // for (int row = chunkStart; row < chunkEnd; ++row)
    // {
    //     if (row % 2 == 0)
    //     {
    //         // Even rows: left to right
    //         const int chunkSizeY = 8;
    //         int numChunks = vecWidth / chunkSizeY;
    //         int const maxChunks = numChunks;

    //         while (numChunks >= 0)
    //         {
    //             for (int col = chunkSizeY * numChunks; col < (chunkSizeY * numChunks) + 8; ++col)
    //             {
    //                 updateSequence(vecWidth, vecHeight, row, col, vec);
    //             }
    //             numChunks--;
    //         }

    //         numChunks = maxChunks - 1;
    //         while (numChunks >= 0)
    //         {
    //             for (int col = chunkSizeY * numChunks; col < (chunkSizeY * numChunks) + 8; ++col)
    //             {
    //                 updateSequence(vecWidth, vecHeight, row, col, vec);
    //             }
    //             numChunks -= 2;
    //         }
    //     }
    //     else
    //     {
    //         // Odd rows: right to left
    //         const int chunkSizeY = 8;
    //         int numChunks = vecWidth / chunkSizeY;
    //         int const maxChunks = numChunks;
    //         while (numChunks >= 0)
    //         {
    //             for (int col = chunkSizeY * numChunks; col < (chunkSizeY * numChunks) + 8; ++col)
    //             {
    //                 updateSequence(vecWidth, vecHeight, row, col, vec);
    //             }
    //             numChunks -= 2;
    //         }
    //         numChunks = maxChunks - 1;
    //         while (numChunks >= 0)
    //         {
    //             for (int col = chunkSizeY * numChunks; col < (chunkSizeY * numChunks) + 8; ++col)
    //             {
    //                 updateSequence(vecWidth, vecHeight, row, col, vec);
    //             }
    //             numChunks -= 2;
    //         }
    //     }
    // }
}

void Game::updateSequence(int &vecWidth, int &vecHeight, int &row, int &col, Chunk &vec)
{
    if (vec[row][col] == nullptr)
        return;
    if (vec[row][col]->getProcessed())
        return;
    vec[row][col]->setProcessed(true);
    if (vec[row][col]->getIsFlammable())
    {
        if (vec[row][col]->fireTick(vec, row, col, vecHeight, smoke))
        {
            vec[row][col] = smoke->clone();
        };
    }


    Vector2D globalCoords = vec.getGlobalCoords();
    
    // Chunk rightChunk;
    // Chunk leftChunk;
    // Chunk topChunk;
    // Chunk bottomChunk;
    // rightChunk = worldGeneration.getChunk(Vector2D(globalCoords.x+1, globalCoords.y));
    // leftChunk = worldGeneration.getChunk(Vector2D(globalCoords.x-1, globalCoords.y));
    // topChunk = worldGeneration.getChunk(Vector2D(globalCoords.x, globalCoords.y - 1));
    // bottomChunk = worldGeneration.getChunk(Vector2D(globalCoords.x, globalCoords.y + 1));
    
    vec[row][col]->update(vec, row, col, vecWidth, vecHeight, worldGeneration.getChunk(Vector2D(globalCoords.x-1, globalCoords.y)), worldGeneration.getChunk(Vector2D(globalCoords.x+1, globalCoords.y)), worldGeneration.getChunk(Vector2D(globalCoords.x, globalCoords.y + 1)), worldGeneration.getChunk(Vector2D(globalCoords.x, globalCoords.y - 1)));
}

void Game::worker(int startingChunkRow, int startingChunkCol, int numChunksY, int numChunksX, int chunkSizeY, int chunkSizeX, int vecHeight, int vecWidth, Chunk &vec, int rowChunk, int colChunk)
{
    // Calculate the boundaries of the current chunk
    int rowStart = rowChunk * chunkSizeY;
    int colStart = colChunk * chunkSizeX;
    int rowEnd = std::min(rowStart + chunkSizeY, vecHeight);
    int colEnd = std::min(colStart + chunkSizeX, vecWidth);

    // Update the 8x8 chunk
    for (int row = rowStart; row < rowEnd; ++row)
    {
        for (int col = colStart; col < colEnd; ++col)
        {
            updateSequence(vecWidth, vecHeight, row, col, vec);
        }
    }
}

void Game::ChunkUpdateSkipping(int startingChunkRow, int startingChunkCol, int numChunksY, int numChunksX, int chunkSizeY, int chunkSizeX, int vecHeight, int vecWidth, Chunk &vec)
{
    std::vector<std::future<void>> futures;

    // Enqueue tasks
    for (int rowChunk = startingChunkRow; rowChunk < numChunksY; rowChunk += 2)
    {
        for (int colChunk = startingChunkCol; colChunk < numChunksX; colChunk += 2)
        {
            auto future = threads.enqueue([this, startingChunkRow, startingChunkCol, numChunksY, numChunksX, chunkSizeY, chunkSizeX, vecHeight, vecWidth, &vec, rowChunk, colChunk]()
            {
                this->worker(startingChunkRow, startingChunkCol, numChunksY, numChunksX, chunkSizeY, chunkSizeX, vecHeight, vecWidth, vec, rowChunk, colChunk);
            });
            futures.push_back(std::move(future));  // Move future into vector
        }
    }
    for (auto& future : futures)
    {
        future.get();  // This will wait for each task to complete
    }
}

void Game::update(const int &xScale, const int &yScale)
{
    const int chunkSizeX = 48;
    const int chunkSizeY = 48;
    int numChunksX = vecWidth / chunkSizeX;
    int numChunksY = vecHeight / chunkSizeY;

    Chunk vec = worldGeneration.getLocalVec();


    Vector2D coords = player->getCoordinates();
    Vector2D dimensions = player->getDimensions();
    std::map<Vector2D, Chunk>& chunks = worldGeneration.getVecStore();
    for (auto& mapEntry : chunks) {
        Chunk& vec2D = mapEntry.second;
        Vector2D globalCoords = mapEntry.first;
        ChunkUpdateSkipping(1, 1, numChunksX, numChunksY, chunkSizeY, chunkSizeX, vecHeight, vecWidth, vec2D);
        ChunkUpdateSkipping(1, 0, numChunksX, numChunksY, chunkSizeY, chunkSizeX, vecHeight, vecWidth, vec2D);
        ChunkUpdateSkipping(0, 1, numChunksX, numChunksY, chunkSizeY, chunkSizeX, vecHeight, vecWidth, vec2D);
        ChunkUpdateSkipping(0, 0, numChunksX, numChunksY, chunkSizeY, chunkSizeX, vecHeight, vecWidth, vec2D);
    }
    player->update(vec, Rendering::getRenderer(), vecWidth);

    worldGeneration.getGlobalCoordinates(coords);
}

void Game::render()
{
    std::map<Vector2D, Chunk> temp = worldGeneration.getVecStore();
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
