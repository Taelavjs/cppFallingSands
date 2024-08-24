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
#include "utility/ProceduralTerrainGen.hpp"

class Sprite;
class Pixel;
class Game;
constexpr double GLOBAL_CONST_VAR{3.5f};

const uint8_t rendererScalex{4};
const uint8_t rendererScaley{4};
const int vecWidth{48 * 4};
const int vecHeight{48 * 4};
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



    // TESTING TEXTURES
void textureTesting(){
        SDL_Window* window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 500, 500, SDL_WINDOW_SHOWN );
        SDL_Renderer* renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );
        char* testPath{"Sprites/AnimationSheet_Character.png"};
        int width{32};
        int height{32};
        int rows{6};
        int cols{6};
        Sprite* testSheet = new Sprite(testPath, width, height, rows, cols);
 
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

    std::vector<float> pixels(vecWidth * vecHeight);
    Game game(vecWidth + 1, vecHeight + 1);
    game.init(&title, rendererScalex, rendererScaley);

    const int fps = 60;
    const float timeBetweenFrames = 1.0f / fps;
    static Timer system_timer;
    float accumulated_seconds{ 0.0f };
    // Uint32 frameStart{};
    // int frameTime{};
    int i{0};
    const bool debug = true;
    if (debug){
        pixels = ProceduralTerrainGen::createNoise(vecWidth, vecHeight);
        game.generateTerrain(pixels);
        pixels = ProceduralTerrainGen::createTerrain(vecWidth, vecHeight);
        game.generateCorridors(pixels);

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
