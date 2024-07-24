#include "Rendering.hpp"

Rendering::Rendering(int vecWidth, int vecHeight, const std::string* title, int scaleX, int scaleY) 
    :    rendererScalex(scaleX), rendererScaley(scaleY), screenHeight(vecHeight), screenWidth(vecWidth)
{

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
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
}

Rendering::~Rendering()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Rendering::renderGrid(std::vector<std::vector<Pixel *>> &vec){
    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, screenWidth, screenHeight);
    if (texture == nullptr)
    {
        std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
        return;
    }
    uint32_t *pixels = new uint32_t[screenWidth * screenHeight];

    for (int row = 0; row < screenWidth; ++row)
    {
        for (int col = 0; col < screenWidth; ++col)
        {
            uint32_t color = (vec[row][col] != nullptr) ? vec[row][col] -> getColour() : SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32), 0, 25, 0, 255);
            pixels[row * screenWidth + col] = color;

            if (vec[row][col] == nullptr)
                continue;
            vec[row][col]->setProcessed(false);
        }
    }
    SDL_UpdateTexture(texture, NULL, pixels, screenWidth * sizeof(uint32_t));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);

    // Clean up
    delete[] pixels;
    SDL_DestroyTexture(texture);
}
