#include "Game.hpp"

Game::Game(int vecWidth, int vecHeight)
    : vecWidth(vecWidth), vecHeight(vecHeight),
      isRunning(true), vec(vecHeight, std::vector<Pixel *>(vecWidth)) {
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
                vec[row][col] = sand -> clone(); // Create a Sand object and store its pointer
            }
        }
    }
}

void SquarePlace(std::vector<std::vector<Pixel *>> &vec, int x, int y, Pixel *elm){
    for (int j = x - 30; j < x + 30; j++) {
        for (int k = y - 30; k < y + 30; k++) {
            if (j < 0 || k < 0 || k >= vec.size() || j >= vec[k].size()) continue;
            if(rand() % 2 == 0) continue;

            delete vec[k][j];  // Delete existing object to avoid memory leak
            vec[k][j] = elm->clone();  // Assign new clone
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
            if(e.key.keysym.sym == SDLK_a){
                SquarePlace(vec, x/xScale, y/yScale, water);
            } else if(e.key.keysym.sym == SDLK_d){
                SquarePlace(vec, x/xScale, y/yScale, rock);
            } else if(e.key.keysym.sym == SDLK_s){
                SquarePlace(vec, x/xScale, y/yScale, sand);
            } else if(e.key.keysym.sym == SDLK_w){
                SquarePlace(vec, x/xScale, y/yScale, smoke);
            } else if(e.key.keysym.sym == SDLK_q){
                SquarePlace(vec, x/xScale, y/yScale, oil);
            }
        }

        
    }
}


void Game::update()
{
    for (int row = 0; row < vecHeight; ++row)
    {
        if (row % 2 == 0)
        {
            // Even rows: left to right
            for (int col = 0; col < vecWidth; ++col)
            {
                if (vec[row][col] == nullptr)
                    continue;
                if (vec[row][col]->getProcessed())
                    continue;
                vec[row][col]->setProcessed(true);
                vec[row][col]->update(vec, row, col, vecWidth, vecHeight);
            }
        }
        else
        {
            // Odd rows: right to left
            for (int col = vecWidth - 1; col >= 0; --col)
            {
                if (vec[row][col] == nullptr)
                    continue;
                if (vec[row][col]->getProcessed())
                    continue;
                vec[row][col]->setProcessed(true);
                vec[row][col]->update(vec, row, col, vecWidth, vecHeight);
            }
        }
    }
}

void Game::render()
{
    rendering -> renderGrid(vec);
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
