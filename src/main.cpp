#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <random>
#include "SDL2/SDL.h"
#include "./GameSetup/Game.hpp"
#include "./Elements/BaseElements/Pixel.hpp"

int main(int argc, char *argv[])
{
    const uint8_t rendererScalex{1};
    const uint8_t rendererScaley{1};
    const int vecWidth{250};
    const int vecHeight{250};
    const std::string title {"awesomepawsome"};

    Game game(vecWidth, vecHeight, rendererScalex, rendererScaley);
    game.init(&title);

    const int fps = 30;
    const int timeBetweenFrames = 1000/fps;
    Uint32 frameStart{};
    int frameTime{};
    int i{0};


    while (i < 1000 && game.getRunning())
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
        if(frameTime < timeBetweenFrames){
            SDL_Delay(timeBetweenFrames - frameTime);
        }
    }

    SDL_Quit();
    return 0;
}
