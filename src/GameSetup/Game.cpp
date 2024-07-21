#include "Game.hpp"


Game::Game(int vecWidth, int vecHeight, int rendererScalex, int rendererScaley)
    : vecWidth(vecWidth), vecHeight(vecHeight),
      rendererScalex(rendererScalex), rendererScaley(rendererScaley),
      isRunning(true), vec(vecHeight, std::vector<Pixel *>(vecWidth)) {}

Game::~Game() {
    for(int i = vecHeight-1; i >= 0; i--){
        for(int k = vecWidth-1; k >= 0; k--){
            if(vec[i][k] == nullptr) continue;
            delete vec[i][k];
        }
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Game::init(const std::string* title) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL Initialization failed: " << SDL_GetError() << std::endl;
        return;
    }
    std::cout << "Everything SDL Initialized Correctly" << '\n';

    window = SDL_CreateWindow(title->c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, vecWidth * rendererScalex, vecHeight * rendererScaley, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_RenderSetScale(renderer, rendererScalex, rendererScaley);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawPoint(renderer, 5, 5);
    SDL_RenderPresent(renderer);
    SDL_PumpEvents();

    for (int row = 0; row < vecHeight; ++row) {
        for (int col = 0; col < vecWidth; ++col) {
            if (randomnumber() > 0.35) {
                vec[row][col] = new Sand();
                //vec[row][col].setType(PixelType::Sand);
            }
        }
    }
}

void Game::handleEvents(const uint8_t &xScale, const uint8_t &yScale) {
    SDL_Event e;
    int x{}, y{};
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            setRunning(false);
        }
        if (e.type == SDL_KEYDOWN) {
            setRunning(false);
        }
        if (e.type == SDL_MOUSEBUTTONDOWN) {
            SDL_GetMouseState(&x, &y);
            vec[y / xScale][x / yScale] = new Sand();
        }
    }
}

void Game::update() {
    for (int row = vecHeight - 1; row >= 0; --row) {
        for (int col = 0; col < vecWidth; ++col) {
            if (vec[row][col] == nullptr) continue;

            if (row + 1 >= vecHeight) continue;
            Sand* sand = static_cast<Sand*>(vec[row][col]);

            sand -> update(vec, row, col, vecWidth, vecHeight);

        }
    }
}


void Game::render() {
    for (int row = 0; row < vecHeight; ++row) {
        for (int col = 0; col < vecWidth; ++col) {
            int color = (vec[row][col] != nullptr) ? 1 : 0;

            SDL_SetRenderDrawColor(renderer, 255 * color, 255 * color, 255 * color, 255);
            SDL_RenderDrawPoint(renderer, col, row);
        }
    }
    SDL_RenderPresent(renderer);
    SDL_PumpEvents();
};

void Game::clean() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
};

double Game::randomnumber(){
    static std::default_random_engine rng;
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    return dist(rng);

}
