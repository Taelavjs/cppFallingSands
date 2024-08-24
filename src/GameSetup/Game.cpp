#include "Game.hpp"

Game::Game(int vecWidthInp, int vecHeightInp)
    : vecWidth(vecWidthInp-1), vecHeight(vecHeightInp-1),
      isRunning(true), vec(vecHeightInp, std::vector<Pixel *>(vecWidthInp))
{
    sand = new Sand();
    water = new Water();
    rock = new Rock();
    smoke = new Smoke();
    oil = new Oil();
    napalm = new Napalm();
}

Game::~Game()
{
    for (int i = vecHeight - 1; i >= 0; i--)
    {
        for (int k = vecWidth - 1; k >= 0; k--)
        {
            if (vec[i][k] == nullptr)
                continue;
            delete vec[i][k];
        }
    }
    // SDL_DestroyRenderer(renderer);
    // SDL_DestroyWindow(window);
    // SDL_Quit();
    delete sand;
    delete water;
    delete rock;
    delete player;
}

void Game::init(const std::string *title, int scaleX, int scaleY)
{
    char* playerSpritePath{"Sprites/AnimationSheet_Character.png"};
    int width{32};
    int height{32};
    int rows{6};
    int cols{6};
    Rendering::setValues(vecWidth, vecHeight, title, scaleX, scaleY);
    Sprite* playerSprite = new Sprite(playerSpritePath, width, height, rows, cols);
    player = new Player(playerSprite);

    for (int row = 0; row < vecHeight; ++row)
    {
        for (int col = 0; col < vecWidth; ++col)
        {
            if (randomnumber() < 0.0)
            {
                vec[row][col] = sand->clone();
            }
        }
    }
}

void SquarePlace(std::vector<std::vector<Pixel *>> &vec, int x, int y, Pixel *elm)
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
    SDL_Event e;
    int x{}, y{};
    while (SDL_PollEvent(&e))
    {

        if (e.type == SDL_QUIT)
        {
            setRunning(false);
        }
        if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            setRunning(false);
        }
        if (e.type == SDL_KEYDOWN)
        {
            SDL_GetMouseState(&x, &y);
            if (e.key.keysym.sym == SDLK_a)
            {
                SquarePlace(vec, x / xScale, y / yScale, water);
            }
            else if (e.key.keysym.sym == SDLK_d)
            {
                SquarePlace(vec, x / xScale, y / yScale, rock);
            }
            else if (e.key.keysym.sym == SDLK_s)
            {
                SquarePlace(vec, x / xScale, y / yScale, sand);
            }
            else if (e.key.keysym.sym == SDLK_w)
            {
                SquarePlace(vec, x / xScale, y / yScale, smoke);
            }
            else if (e.key.keysym.sym == SDLK_q)
            {
                SquarePlace(vec, x / xScale, y / yScale, oil);
            } else if (e.key.keysym.sym == SDLK_f) {
                SquarePlace(vec, x / xScale, y / yScale, napalm);
            }

            player->playerInputHandler(e);
        } else if(e.type == SDL_KEYUP){
            player->playerReleaseHandler(e);
        }
    }
}

void Game::chunkUpdates(int chunkStart, int chunkEnd){


    for (int row = chunkStart; row < chunkEnd; ++row)
    {
        if (row % 2 == 0)
        {
            // Even rows: left to right
            const int chunkSizeY = 8;
            int numChunks = vecWidth / chunkSizeY;
            int const maxChunks = numChunks;

            while(numChunks >= 0){
                for (int col = chunkSizeY * numChunks; col < (chunkSizeY * numChunks) + 8; ++col)
                {
                    updateSequence(vecWidth, vecHeight, row, col, vec);
                }
                numChunks--;
            }

            numChunks = maxChunks - 1;
            while(numChunks >= 0){
                for (int col = chunkSizeY * numChunks; col < (chunkSizeY * numChunks) + 8; ++col)
                {
                    updateSequence(vecWidth, vecHeight, row, col, vec);
                }
                numChunks -= 2;
            }
        }
        else
        {
            // Odd rows: right to left
            const int chunkSizeY = 8;
            int numChunks = vecWidth / chunkSizeY;
            int const maxChunks = numChunks;
            while(numChunks >= 0){
                for (int col = chunkSizeY * numChunks; col < (chunkSizeY * numChunks) + 8; ++col)
                {
                    updateSequence(vecWidth, vecHeight, row, col, vec);
                }
                numChunks-=2;
            }
            numChunks = maxChunks - 1;
            while(numChunks >= 0){
                for (int col = chunkSizeY * numChunks; col < (chunkSizeY * numChunks) + 8; ++col)
                {
                    updateSequence(vecWidth, vecHeight, row, col, vec);
                }
                numChunks-=2;
            }
        }
    }
}

void Game::updateSequence(int &vecWidth, int &vecHeight, int &row, int &col, std::vector<std::vector<Pixel *>> &vec)
{
    if (vec[row][col] == nullptr)
        return;
    if (vec[row][col]->getProcessed())
        return;
    vec[row][col]->setProcessed(true);
    if(vec[row][col]->getIsFlammable()){
        if(vec[row][col]->fireTick(vec, row, col, vecHeight, smoke)){
            vec[row][col] = smoke->clone();
        };
    }
    vec[row][col]->update(vec, row, col, vecWidth, vecHeight);
}

void checkCollision(std::vector<std::vector<Pixel*>> &vec, int x, int y, int playerWidth, int playerHeight, int vecWidth, Player* playerObj) {
    // SDL_Rect player = {x+4, y+4, playerWidth -8 , playerHeight - 8}; // leniency

    // for(int i = x - 30; i <= x + 30; ++i) {
    //     for(int j = y - 30; j <= y + 30; ++j) {
    //         if (j < 0 || j >= vecWidth || i < 0 || i >= vecWidth) continue;

    //         if (vec[j][i] != nullptr && vec[j][i] -> getIsSolid()) {
    //             SDL_Rect pxl = {i, j, 1, 1}; 

    //             if (SDL_HasIntersection(&pxl, &player)) {
    //                 SDL_Rect tmp = {pxl.x, pxl.y, pxl.w, pxl.h};
    //                 SDL_Rect tmp2 = {player.x, player.y, player.w, player.h};
    //                 SDL_Rect result;  // No need for dynamic allocation

    //                 // Calculate the intersection
    //                 if (SDL_IntersectRect(&tmp, &tmp2, &result)) {
    //                     playerObj->handleCollision(&result);


    //                 }

    //                     std::tuple coords = playerObj->getCoordinates();
    //                     std::tuple dimensions = playerObj->getDimensions();

    //                     int playerY = std::get<1>(coords); //swapped since vec x and y are swapped
    //                     int playerX = std::get<0>(coords);
    //                     int scaleY = std::get<1>(dimensions);
    //                     int scaleX = std::get<0>(dimensions); 

    //                     player = {playerX+2, playerY+2, scaleX -4 , scaleY - 4}; // leniency

    //             }
    //         }
    //     }
    // }
}

void Game::worker(int startingChunkRow, int startingChunkCol, int numChunksY, int numChunksX, int chunkSizeY, int chunkSizeX, int vecHeight, int vecWidth, std::vector<std::vector<Pixel*>> &vec, int rowChunk, int colChunk){
            // Calculate the boundaries of the current chunk
    int rowStart = rowChunk * chunkSizeY;
    int colStart = colChunk * chunkSizeX;
    int rowEnd = std::min(rowStart + chunkSizeY, vecHeight);
    int colEnd = std::min(colStart + chunkSizeX, vecWidth);

    // Update the 8x8 chunk
    for(int row = rowStart; row < rowEnd; ++row) {
        for(int col = colStart; col < colEnd; ++col) {
            updateSequence(vecWidth, vecHeight, row, col, vec);
        }
    }
}

void Game::ChunkUpdateSkipping(int startingChunkRow, int startingChunkCol, int numChunksY, int numChunksX, int chunkSizeY, int chunkSizeX, int vecHeight, int vecWidth, std::vector<std::vector<Pixel*>> &vec){
    std::vector<std::thread> threads;
    for(int rowChunk = startingChunkRow; rowChunk < numChunksY; rowChunk += 2) {
        for(int colChunk = startingChunkCol; colChunk < numChunksX; colChunk += 2) {
            threads.emplace_back([this, startingChunkRow, startingChunkCol, numChunksY, numChunksX, chunkSizeY, chunkSizeX, vecHeight, vecWidth, &vec, rowChunk, colChunk]() {
                this->worker(startingChunkRow, startingChunkCol, numChunksY, numChunksX, chunkSizeY, chunkSizeX, vecHeight, vecWidth, vec, rowChunk, colChunk);
            }); 
        }
    }

    for (auto& t : threads) { 
        t.join(); 
    } 
}


void Game::update( const int& xScale, const int& yScale)
{
    const int chunkSizeX = 48;
    const int chunkSizeY = 48;
    int numChunksX = vecWidth / chunkSizeX;
    int numChunksY = vecHeight / chunkSizeY;

    ChunkUpdateSkipping(1, 1, numChunksX, numChunksY, chunkSizeY, chunkSizeX, vecHeight, vecWidth, vec);
    ChunkUpdateSkipping(1, 0, numChunksX, numChunksY, chunkSizeY, chunkSizeX, vecHeight, vecWidth, vec);
    ChunkUpdateSkipping(0, 1, numChunksX, numChunksY, chunkSizeY, chunkSizeX, vecHeight, vecWidth, vec);
    ChunkUpdateSkipping(0, 0, numChunksX, numChunksY, chunkSizeY, chunkSizeX, vecHeight, vecWidth, vec);


    std::tuple coords = player->getCoordinates();
    std::tuple dimensions = player->getDimensions();

    int playerY = std::get<1>(coords); //swapped since vec x and y are swapped
    int playerX = std::get<0>(coords);
    int scaleY = std::get<1>(dimensions);
    int scaleX = std::get<0>(dimensions);
    //checkCollision(vec, playerX, playerY, scaleX, scaleY, vecHeight, player);
    player->update(vec, Rendering::getRenderer(), vecWidth);

}

void Game::render()
{
    Rendering::renderGrid(vec, player);
}

void Game::clean()
{
};

double Game::randomnumber()
{
    static std::default_random_engine rng;
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    return dist(rng);
}

void Game::generateTerrain(std::vector<float> pixels){
    for(int row = 0; row < vecHeight; ++row){
        for (int col = 0; col < vecWidth; ++col){
            const float pixValue = pixels[row * vecWidth + col];
            if(pixValue > -0.2f){
                vec[row][col] = rock->clone();

            }
        }
    }
}

void Game::generateCorridors(std::vector<float> pixels){
    for(int row = 0; row < vecHeight; ++row){
        for (int col = 0; col < vecWidth; ++col){
            const float pixValue = pixels[row * vecWidth + col];
            if(vec[row][col] != nullptr && vec[row][col]->getIsSolid() && pixValue > 0.6){
                vec[row][col] = nullptr;
            } 

            if(row < 30){
                vec[row][col] = nullptr;
            }
        }
    }
}
