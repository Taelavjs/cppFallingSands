#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <random>
#include "SDL2/SDL.h"
#include "./GameSetup/Game.hpp"
#include "./Elements/BaseElements/Pixel.hpp"
constexpr double GLOBAL_CONST_VAR{3.5f};

const uint8_t rendererScalex{5};
const uint8_t rendererScaley{5};
const int vecWidth{79};
const int vecHeight{79};
const std::string title{"awesomepawsome"};

void ProceduralGeneratedMap()
{
    SDL_Window *window = SDL_CreateWindow("title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, vecWidth * rendererScalex, vecHeight * rendererScaley, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_RenderSetScale(renderer, rendererScalex, rendererScaley);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderPresent(renderer);
    SDL_PumpEvents();

    const int blackColor = SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32), 0, 0, 0, 255);
    const int whiteColor = SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32), 255, 255, 255, 255);

    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, vecWidth, vecHeight);
    if (texture == nullptr)
    {
        std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
        return;
    }
    uint32_t *pixels = new uint32_t[vecWidth * vecHeight];
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 1); // uniform distribution between 0 and 1

    for (int row = 0; row < vecWidth; ++row)
    {
        for (int col = 0; col < vecWidth; ++col)
        {
            uint32_t color{};
            if (dis(gen) > 0.40)
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

    for (int i = 0; i < 8; ++i)
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
                        if (i == 0 && j == 0) continue;
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
                if (neighborCount < 4)
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

        std::swap(pixels, newPixels);
        SDL_UpdateTexture(texture, NULL, newPixels, vecWidth * sizeof(uint32_t));
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
        SDL_PumpEvents();
        SDL_Delay(500);
    }

    delete[] pixels;
    delete[] newPixels;
    SDL_DestroyTexture(texture);
    SDL_Delay(2000);
}

int main(int argc, char *argv[])
{

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
        ProceduralGeneratedMap();

    SDL_Quit();
    return 0;
}
