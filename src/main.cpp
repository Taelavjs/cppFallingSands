#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <random>
#include "SDL2/SDL.h"
#include <SDL2/SDL_image.h>
#include "./GameSetup/Game.hpp"
#include "./Elements/BaseElements/Pixel.hpp"
#include "Textures/Sprite.hpp"
constexpr double GLOBAL_CONST_VAR{3.5f};

const uint8_t rendererScalex{3};
const uint8_t rendererScaley{3};
const int vecWidth{160};
const int vecHeight{160};
const std::string title{"awesomepawsome"};

const int blackColor = SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32), 0, 0, 0, 255);
const int whiteColor = SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32), 255, 255, 255, 255);
const int redColor = SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32), 255, 0, 0, 255);
const int greenColor = SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32), 0, 255, 0, 255);
const int blueColor = SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32), 0, 0, 255, 255);
const int yellowColor = SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32), 201, 217, 98, 255);
const int vertColor = SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32), 50, 168, 127, 255);

const double liveCellChance = 0.17f;
const int liveNeightborFreshold = 3;
const int seaLevel = 30;

struct Timer
{
    Uint64 previous_ticks{};
    float elapsed_seconds{};

    void tick()
    {
        const Uint64 current_ticks{ SDL_GetPerformanceCounter() };
        const Uint64 delta{ current_ticks - previous_ticks };
        previous_ticks = current_ticks;
        static const Uint64 TICKS_PER_SECOND{ SDL_GetPerformanceFrequency() };
        elapsed_seconds = delta / static_cast<float>(TICKS_PER_SECOND);
    }
};

void displayPixels(SDL_Texture *texture, uint32_t *pixels, SDL_Renderer *renderer){
    SDL_UpdateTexture(texture, NULL, pixels, vecWidth * sizeof(uint32_t));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
    SDL_PumpEvents();
    SDL_Delay(0);
}

void iterThroughPixels(void (*func)(int &, int &, uint32_t *, uint32_t *), uint32_t *pixels, uint32_t *newPixels){
    for (int row = 0; row < vecWidth; ++row)
    {
        for (int col = 0; col < vecWidth; ++col)
        {
            func(row, col, pixels, newPixels);
        }
    }
}
void waterLevelLimit(int &row, int &col, uint32_t *pixels, uint32_t *newPixels){
    uint32_t color{};
    int yellowNeighborCount{0};
    int greenNeighborCount{0};
    int BlueNeighborCount{0};
    for (int i = -1; i <= 1; ++i)
    {
        for (int j = -1; j <= 1; ++j)
        {
            if (i == 0 && j == 0)
                continue;
            int neighborRow = row + i;
            int neighborCol = col + j;

            if (neighborRow >= 0 && neighborRow < vecHeight && neighborCol >= 0 && neighborCol < vecWidth)
            {
                if (pixels[neighborRow * vecWidth + neighborCol] == greenColor)
                {
                    greenNeighborCount++;
                } else if(pixels[neighborRow * vecWidth + neighborCol] == yellowColor){
                    yellowNeighborCount++;

                } else if(pixels[neighborRow * vecWidth + neighborCol] == blueColor){
                    BlueNeighborCount++;
                }
            }
        }
    }

    if (greenNeighborCount < 3)
    {
        color = blueColor;
    }
    else
    {
        color = greenNeighborCount  >= 4 ? greenColor : vertColor;
        if(BlueNeighborCount > 2 && BlueNeighborCount <= 5){
            color = yellowColor;
        } else if(BlueNeighborCount > 2){
            color = blueColor;
        }
    }
    newPixels[row * vecWidth + col] = color;
}
void colorCells(int &row, int &col, uint32_t *pixels, uint32_t *newPixels){
    uint32_t color{};

    int neighborCount{0};
    for (int i = -1; i <= 1; ++i)
    {
        for (int j = -1; j <= 1; ++j)
        {
            if (i == 0 && j == 0)
                continue;
            int neighborRow = row + i;
            int neighborCol = col + j;

            if (neighborRow >= 0 && neighborRow < vecHeight && neighborCol >= 0 && neighborCol < vecWidth)
            {
                if (pixels[neighborRow * vecWidth + neighborCol] == whiteColor || 
                    pixels[neighborRow * vecWidth + neighborCol] == greenColor)
                {
                    neighborCount++;
                }
            }
        }
    }
    if (neighborCount < liveNeightborFreshold)
    {
        color = blueColor;
    }
    else
    {
        color = neighborCount == 8 ? greenColor : yellowColor;
    }

    newPixels[row * vecWidth + col] = color;
}
void placeLiveDeadCells(int &row, int &col, uint32_t *pixels, uint32_t *newPixels){
    uint32_t color{};

    int neighborCount{0};
    for (int i = -1; i <= 1; ++i)
    {
        for (int j = -1; j <= 1; ++j)
        {
            if (i == 0 && j == 0)
                continue;
            int neighborRow = row + i;
            int neighborCol = col + j;

            if (neighborRow >= 0 && neighborRow < vecHeight && neighborCol >= 0 && neighborCol < vecWidth)
            {
                if (pixels[neighborRow * vecWidth + neighborCol] == whiteColor)
                {
                    neighborCount++;
                }
            }
        }
    }
    if (neighborCount < liveNeightborFreshold)
    {
        color = blackColor;
    }
    else
    {
        color = whiteColor;
    }

    newPixels[row * vecWidth + col] = color;
};

void ProceduralGeneratedMap(uint32_t *pixels)
{
    SDL_Window *window = SDL_CreateWindow("title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, vecWidth * rendererScalex, vecHeight * rendererScaley, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_RenderSetScale(renderer, rendererScalex, rendererScaley);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderPresent(renderer);
    SDL_PumpEvents();

    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, vecWidth, vecHeight);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 1); 

    for (int row = 0; row < vecWidth; ++row)
    {
        for (int col = 0; col < vecWidth; ++col)
        {
            uint32_t color{};
            if (dis(gen) > liveCellChance)
            {
                color = blackColor;
            }
            else
            {
                color = whiteColor;
            }

            pixels[row * vecWidth + col] = color;
        }
    }

    displayPixels(texture, pixels, renderer);

    uint32_t *newPixels = new uint32_t[vecWidth * vecHeight];

    for (int i = 0; i < 5; ++i)
    {
        iterThroughPixels(placeLiveDeadCells, pixels, newPixels);
        std::swap(pixels, newPixels);
        displayPixels(texture, pixels, renderer);
    }

    iterThroughPixels(colorCells, pixels, newPixels);
    std::swap(pixels, newPixels);
    displayPixels(texture, pixels, renderer);

    // for(int p = 0; p < 2; ++p){
    //     iterThroughPixels(waterLevelLimit, pixels, newPixels);
    //     std::swap(pixels, newPixels);
    //     displayPixels(texture, pixels, renderer);
    // }

    delete[] newPixels;
    delete[] pixels;
    SDL_DestroyTexture(texture);
    SDL_Delay(0);
    SDL_DestroyWindow(window);
};

    // TESTING TEXTURES
void textureTesting(){
        SDL_Window* window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 500, 500, SDL_WINDOW_SHOWN );
        SDL_Renderer* renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );
        char* testPath{"Sprites/AnimationSheet_Character.png"};
        int width{32};
        int height{32};
        int rows{6};
        int cols{6};
        Sprite* testSheet = new Sprite(testPath, renderer, width, height, rows, cols);
 
        for(int i = 0; i < 10; ++i){
            SDL_RenderCopy(renderer,testSheet->runCycle(), NULL, NULL);
            SDL_RenderPresent(renderer);
            SDL_PumpEvents();
            SDL_Delay(500); 
        }
        delete testSheet;

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
    }

int main(int argc, char *argv[])
{
    char* charSpritePath{"Sprites/AnimationSheet_Character.png"};

    uint32_t *pixels = new uint32_t[vecWidth * vecHeight];
    Game game(vecWidth + 1, vecHeight + 1);
    game.init(&title, rendererScalex, rendererScaley);

    const int fps = 600;
    const float timeBetweenFrames = 1.0f / fps;
    static Timer system_timer;
    float accumulated_seconds{ 0.0f };
    // Uint32 frameStart{};
    // int frameTime{};
    int i{0};
    const bool debug = true;
    if (debug){
        ProceduralGeneratedMap(pixels);
        game.generateTerrain(pixels);
    }
    while (game.getRunning())
    {
        system_timer.tick();
        accumulated_seconds += system_timer.elapsed_seconds;
        if (std::isgreater(accumulated_seconds, timeBetweenFrames)){
            accumulated_seconds = -timeBetweenFrames;

            i++;
            // Events
            game.handleEvents(rendererScalex, rendererScaley);
            // frame runing
            game.update(rendererScalex, rendererScaley);
            // Render
            game.render();
        }
        // frameStart = SDL_GetTicks();

        // frameTime = SDL_GetTicks() - frameStart;
        // if (frameTime < timeBetweenFrames)
        // {
        //     SDL_Delay(timeBetweenFrames - frameTime);
        // }
    }



    SDL_Quit();
    return 0;
}
