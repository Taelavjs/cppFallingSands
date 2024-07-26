#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <random>
#include "SDL2/SDL.h"
#include "./GameSetup/Game.hpp"
#include "./Elements/BaseElements/Pixel.hpp"
constexpr double GLOBAL_CONST_VAR{ 3.5f };

int main(int argc, char *argv[])
{
    const uint8_t rendererScalex{10};
    const uint8_t rendererScaley{10};
    const int vecWidth{60};
    const int vecHeight{60};
    const std::string title {"awesomepawsome"};

    Game game(vecWidth, vecHeight);
    game.init(&title, rendererScalex, rendererScaley);

    const int fps = 60;
    const int timeBetweenFrames = 1000/fps;
    Uint32 frameStart{};
    int frameTime{};
    int i{0};


    while (game.getRunning())
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
