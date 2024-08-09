#include "Rendering.hpp"

Rendering::Rendering(int vecWidth, int vecHeight, const std::string* title, int scaleX, int scaleY) 
    :    rendererScalex(scaleX), rendererScaley(scaleY), screenHeight(vecHeight), screenWidth(vecWidth)
{

    SDL_Init(SDL_INIT_VIDEO);
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

void Rendering::castRay(uint32_t *pixels, SDL_Renderer* renderer, const std::vector<std::vector<Pixel*>>& vec, int screenWidth, int screenHeight, int startX, int startY, int endX, int endY) {
    int dx = abs(endX - startX);
    int dy = abs(endY - startY);
    int sx = startX < endX ? 1 : -1;
    int sy = startY < endY ? 1 : -1;
    int err = dx - dy;
    int e2;
    int count{0};
    int limit{3};
    while (true) {

        if (startX >= 0 && startX < screenWidth && startY >= 0 && startY < screenHeight) {
            if ((vec[startX][startY] == nullptr || vec[startX][startY] -> getIsMoveable())&& limit > 0) {
                limit--;
                pixels[startX * screenWidth + startY] = SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32), 182, 130, 124, 90);
            } else if(vec[startX][startY] != nullptr || limit <= 0) {
                count++;
                if (count==2) break;
            };
        }

        if (startX == endX && startY == endY) break;
        e2 = 2 * err;
        if (e2 > -dy) { err -= dy; startX += sx; }
        if (e2 < dx) { err += dx; startY += sy; }
    }
}


void Rendering::castRays(uint32_t *pixels,SDL_Renderer* renderer, const std::vector<std::vector<Pixel*>>& vec, int screenWidth, int screenHeight, int row, int col) {
    int rays = 360; // Number of rays to cast
    for (int i = 0; i < rays; ++i) {
        float angle = i * (2 * M_PI / rays);
        int endX = row + screenWidth * cos(angle);
        int endY = col + screenHeight * sin(angle);
        castRay(pixels, renderer, vec, screenWidth, screenHeight, row, col, endX, endY);
    }
}

void Rendering::renderGrid(std::vector<std::vector<Pixel *>> &vec, Player* player){
    SDL_RenderClear(renderer);

    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, screenWidth, screenHeight);
    if (texture == nullptr)
    {
        std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
        return;
    }
    uint32_t *pixels = new uint32_t[screenWidth * screenHeight];
    uint32_t blackColor = SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32), 0, 0, 0, 255);

    std::fill(pixels, pixels + (screenWidth * screenHeight), blackColor);

    for (int row = 0; row < screenWidth; ++row)
    {
        for (int col = 0; col < screenWidth; ++col)
        {
            uint32_t color = (vec[row][col] != nullptr) ? vec[row][col] -> getColour() : pixels[row * screenWidth + col];

            pixels[row * screenWidth + col] = color;

            if (vec[row][col] == nullptr)
                continue;
            vec[row][col]->setProcessed(false);
        }

    }
    // X => 0 IS THE RIGHT HAND SIDE ( col )
    // Y => 0 IS THE TOP ( row )

    // for (int row = 0; row < screenWidth; ++row)
    // {
    //     for (int col = 0; col < screenWidth; ++col)
    //     {
    //         if (vec[row][col] == nullptr)
    //             continue;

    //         if(vec[row][col]->getOnFire()){
    //             castRays(pixels, renderer, vec, screenWidth, screenHeight, row, col);
    //         }
    //     }
    // }




    SDL_UpdateTexture(texture, NULL, pixels, screenWidth * sizeof(uint32_t));
    SDL_RenderCopy(renderer, texture, NULL, NULL);

    // Retrieve the texture
    Sprite* playerText = player->getSprite();
    SDL_Texture* playTexture = playerText->runCycle();
    // Create and initialize the destination rectangle
    SDL_Rect* dst = new SDL_Rect{30, 30, 50, 50};

    SDL_RenderCopy(renderer, playTexture, NULL, dst);


    delete[] pixels;
    SDL_DestroyTexture(texture);

    // SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); 
    // for (int y = 0; y < screenWidth; y += 8) {
    //     for (int x = 0; x < screenHeight; x += 8) {
    //         SDL_Rect rect = {x, y, 8, 1};
    //         SDL_RenderFillRect(renderer, &rect);

    //         rect = {x, y, 1, 8};
    //         SDL_RenderFillRect(renderer, &rect); 
    //     }
    // }



    SDL_RenderPresent(renderer);
    delete dst;

}

SDL_Renderer* Rendering::getRenderer(){
    return renderer;
}
