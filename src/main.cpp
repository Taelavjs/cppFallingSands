#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <random>
#include "SDL2/SDL.h"
#include "./GameSetup/Game.hpp"
#include "./Elements/BaseElements/Pixel.hpp"
constexpr double GLOBAL_CONST_VAR{3.5f};

const uint8_t rendererScalex{3};
const uint8_t rendererScaley{3};
const int vecWidth{159};
const int vecHeight{159};
const std::string title{"awesomepawsome"};

const int blackColor = SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32), 0, 0, 0, 255);
const int whiteColor = SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32), 255, 255, 255, 255);
const int redColor = SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32), 255, 0, 0, 255);
const int greenColor = SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32), 0, 255, 0, 255);
const int blueColor = SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32), 0, 0, 255, 255);
const int yellowColor = SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32), 201, 217, 98, 255);
const int vertColor = SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32), 50, 168, 127, 255);

const double liveCellChance = 0.25f;
const int liveNeightborFreshold = 3;
const int seaLevel = 30;

void ProceduralGeneratedMap(uint32_t *pixels)
{
    SDL_Window *window = SDL_CreateWindow("title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, vecWidth * rendererScalex, vecHeight * rendererScaley, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_RenderSetScale(renderer, rendererScalex, rendererScaley);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderPresent(renderer);
    SDL_PumpEvents();

    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, vecWidth, vecHeight);
    if (texture == nullptr)
    {
        std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
        return;
    }
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 1); // uniform distribution between 0 and 1

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
    SDL_UpdateTexture(texture, NULL, pixels, vecWidth * sizeof(uint32_t));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
    SDL_PumpEvents();

    SDL_Delay(500);

    uint32_t *newPixels = new uint32_t[vecWidth * vecHeight];

    for (int i = 0; i < 4; ++i)
    {
        for (int row = 0; row < vecWidth; ++row)
        {
            for (int col = 0; col < vecWidth; ++col)
            {
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
            }
        }

        SDL_UpdateTexture(texture, NULL, newPixels, vecWidth * sizeof(uint32_t));
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
        SDL_PumpEvents();
        SDL_Delay(500);
        std::swap(pixels, newPixels);
    }

    for (int row = 0; row < vecWidth; ++row)
        {
            for (int col = 0; col < vecWidth; ++col)
            {
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
        }
    std::swap(pixels, newPixels);
    
    for(int p = 0; p < 2; ++p){
        for (int row = 0; row < vecWidth; ++row)
        {
            for (int col = 0; col < vecWidth; ++col)
            {
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
                if(row  < vecHeight - seaLevel && color == blueColor){
                    color = blackColor;
                }
                newPixels[row * vecWidth + col] = color;
            }
        }
        SDL_UpdateTexture(texture, NULL, newPixels, vecWidth * sizeof(uint32_t));
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
        SDL_PumpEvents();
        SDL_Delay(500);
        std::swap(pixels, newPixels);
    }

    delete[] newPixels;
    SDL_DestroyTexture(texture);
    SDL_Delay(2000);
}

// ONE






// Two






// Three



int main(int argc, char *argv[])
{
    uint32_t *pixels = new uint32_t[vecWidth * vecHeight];
    Game game(vecWidth + 1, vecHeight + 1);
    game.init(&title, rendererScalex, rendererScaley);

    const int fps = 60;
    const int timeBetweenFrames = 1000 / fps;
    Uint32 frameStart{};
    int frameTime{};
    int i{0};
    const bool debug = true;
    while (!debug && game.getRunning())
    {
        frameStart = SDL_GetTicks();
        i++;
        // Events
        game.handleEvents(rendererScalex, rendererScaley);
        // frame runing
        game.update();
        // Render
        game.render();
        frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < timeBetweenFrames)
        {
            SDL_Delay(timeBetweenFrames - frameTime);
        }
    }
    if (debug)
        ProceduralGeneratedMap(pixels);

    SDL_Quit();
    return 0;
}
