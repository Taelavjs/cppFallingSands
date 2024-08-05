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
}

void Game::init(const std::string *title, int scaleX, int scaleY)
{
    // if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    //     std::cerr << "SDL Initialization failed: " << SDL_GetError() << std::endl;
    //     return;
    // }
    // std::cout << "Everything SDL Initialized Correctly" << '\n';

    // window = SDL_CreateWindow(title->c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, vecWidth * rendererScalex, vecHeight * rendererScaley, SDL_WINDOW_SHOWN);
    // renderer = SDL_CreateRenderer(window, -1, 0);
    // SDL_RenderSetScale(renderer, rendererScalex, rendererScaley);
    // SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    // SDL_RenderDrawPoint(renderer, 5, 5);
    // SDL_RenderPresent(renderer);
    // SDL_PumpEvents();
    rendering = new Rendering(vecWidth, vecHeight, title, scaleX, scaleY);

    for (int row = 0; row < vecHeight; ++row)
    {
        for (int col = 0; col < vecWidth; ++col)
        {
            if (randomnumber() < 0.0)
            {
                vec[row][col] = sand->clone(); // Create a Sand object and store its pointer
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
            }
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
    vec[row][col]->update(vec, row, col, vecWidth, vecHeight);
}

void Game::update()
{
    const int chunkSizeX = 8;
    int numChunks = vecWidth / chunkSizeX;
    int const maxChunks = numChunks;
    while(numChunks >= 0){
        chunkUpdates(chunkSizeX * numChunks, (chunkSizeX * numChunks) + chunkSizeX);
        numChunks -= 2;
    }
    numChunks = maxChunks - 1;
    while(numChunks >= 0){
        chunkUpdates(chunkSizeX * numChunks, (chunkSizeX * numChunks) + chunkSizeX);
        numChunks -= 2;
    }
}

void Game::render()
{
    rendering->renderGrid(vec);
}

void Game::clean()
{
    delete rendering;
    // SDL_DestroyRenderer(renderer);
    // SDL_DestroyWindow(window);
    // SDL_Quit();
};

double Game::randomnumber()
{
    static std::default_random_engine rng;
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    return dist(rng);
}

void Game::generateTerrain(uint32_t *pixels){
    const int blackColor = SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32), 0, 0, 0, 255);
    const int whiteColor = SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32), 255, 255, 255, 255);
    const int redColor = SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32), 255, 0, 0, 255);
    const int greenColor = SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32), 0, 255, 0, 255);
    const int blueColor = SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32), 0, 0, 255, 255);
    const int yellowColor = SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32), 201, 217, 98, 255);
    const int vertColor = SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32), 50, 168, 127, 255);

    for(int row = 0; row < vecHeight; ++row){
        for (int col = 0; col < vecWidth; ++col){
            const int pixColor = pixels[row * vecWidth + col];
            if (pixColor == blueColor){
                vec[row][col] = water->clone();
            } else if (pixColor == greenColor){
                vec[row][col] = sand->clone();
            } else{
                vec[row][col] = rock->clone();
            }
        }
    }
}
