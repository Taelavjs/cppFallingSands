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
            if (randomnumber() < 0.0) {
                vec[row][col] = new Sand(); // Create a Sand object and store its pointer
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

            // Convert mouse position to grid position
            int gridX = x / xScale;
            int gridY = y / yScale;

            // Place Water in a circle around the clicked position
            for (int angle = 0; angle < 360; ++angle) {
                // Convert angle to radians
                double radians = angle * (M_PI / 180.0);
                
                // Calculate offset from the center based on the angle and radius
                int offsetX = static_cast<int>(20 * cos(radians));
                int offsetY = static_cast<int>(20 * sin(radians));
                
                // Calculate grid cell position
                int newX = gridX + offsetX;
                int newY = gridY + offsetY;

                // Ensure we are within grid bounds
                if (newX >= 0 && newY >= 0 && newX < vecWidth && newY < vecHeight) {
                    vec[newY][newX] = new Water();
                }
            }
        }
    }
}

void Game::update() {
for (int row = vecHeight - 1; row >= 0; --row) {
    if (row % 2 == 0) {
        // Even rows: left to right
        for (int col = 0; col < vecWidth; ++col) {
            if (vec[row][col] == nullptr) continue;
            if (vec[row][col]->getProcessed()) continue;
            vec[row][col]->setProcessed(true);
            vec[row][col]->update(vec, row, col, vecWidth, vecHeight);
        }
    } else {
        // Odd rows: right to left
        for (int col = vecWidth - 1; col >= 0; --col) {
            if (vec[row][col] == nullptr) continue;
            if (vec[row][col]->getProcessed()) continue;
            vec[row][col]->setProcessed(true);
            vec[row][col]->update(vec, row, col, vecWidth, vecHeight);
        }
    }
}
}


void Game::render() {
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, vecWidth, vecHeight);
    if (texture == nullptr) {
        std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
        return;
    }
    uint32_t* pixels = new uint32_t[vecWidth * vecHeight];

    for (int row = 0; row < vecHeight; ++row) {
        for (int col = 0; col < vecWidth; ++col) {
            uint32_t color = (vec[row][col] != nullptr) ? SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32), 255, 130, 255, 255) : SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32), 93, 0, 93, 255);
            pixels[row * vecWidth + col] = color;

            if(vec[row][col] == nullptr) continue;
            vec[row][col] -> setProcessed(false);
        }
    }
    SDL_UpdateTexture(texture, NULL, pixels, vecWidth * sizeof(uint32_t));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);

    // Clean up
    delete[] pixels;
    SDL_DestroyTexture(texture);
}


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
