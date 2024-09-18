#include "Rendering.hpp"

SDL_Window* Rendering::window = nullptr;
SDL_Renderer* Rendering::renderer = nullptr;
int Rendering::rendererScalex = 1;
int Rendering::rendererScaley = 1;
int Rendering::screenHeight = 600;  // Default values
int Rendering::screenWidth = 800;   // Default values
int Rendering::offsetX = 0;
int Rendering::offsetY = 0;
const std::string* Rendering::title = nullptr;

void Rendering::setValues(int vecWidth, int vecHeight, const std::string* title, int scaleX, int scaleY){
    Rendering::screenWidth = vecWidth;
    Rendering::screenHeight = vecHeight;
    Rendering::rendererScalex = scaleX;
    Rendering::rendererScaley = scaleY;

    Rendering::title = title;
    SDL_Init(SDL_INIT_VIDEO);
    std::cout << "Everything SDL Initialized Correctly" << '\n';

    Rendering::window = SDL_CreateWindow(Rendering::title->c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Rendering::screenWidth * Rendering::rendererScalex, Rendering::screenHeight * Rendering::rendererScaley, SDL_WINDOW_SHOWN);
    Rendering::renderer = SDL_CreateRenderer(Rendering::window, -1, 0);
    SDL_RenderSetScale(Rendering::renderer, Rendering::rendererScalex, Rendering::rendererScaley);
    SDL_SetRenderDrawColor(Rendering::renderer, 255, 255, 255, 255);
    SDL_RenderDrawPoint(Rendering::renderer, 5, 5);
    SDL_RenderPresent(Rendering::renderer);
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

void Rendering::renderGrid(Chunk &vec, Player* player, Vector2D globalCoords){
    //SDL_RenderClear(renderer);

    int globalOffputX = globalCoords.x * screenWidth;
    int globalOffputY = globalCoords.y * screenWidth;

    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, screenWidth, screenHeight);
    if (texture == nullptr)
    {
        std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
        return;
    }
    uint32_t *pixels = new uint32_t[screenWidth * screenHeight];
    uint32_t blackColor = SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32), 0, 0, 30, 255);

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
    SDL_Rect AABB = player->getPlayerRect();
    Rendering::offsetX = AABB.x - 5;
    Rendering::offsetY = AABB.y - 1;
    SDL_Rect dstRect = {((Rendering::screenWidth/2)-Rendering::offsetX) + globalOffputX,( (Rendering::screenWidth/2)-Rendering::offsetY)  +globalOffputY, screenWidth, screenWidth};

    SDL_UpdateTexture(texture, NULL, pixels, screenWidth * sizeof(uint32_t));
    SDL_RenderCopy(renderer, texture, NULL, &dstRect);
    delete[] pixels;
    SDL_DestroyTexture(texture);
}

void Rendering::renderPlayer(Player* player){
    SDL_Rect AABB = player->getPlayerRect();
    player->renderPlayer(renderer, Rendering::screenHeight);
    std::stack<SDL_Rect> toRender = player->getStackRender();
    while(!toRender.empty()){
        SDL_Rect cube = toRender.top();
        SDL_RenderDrawRect(renderer, &cube);
        toRender.pop();
    }
}

void Rendering::showRendering(){
    SDL_RenderPresent(renderer);
    SDL_RenderClear(renderer);
}

