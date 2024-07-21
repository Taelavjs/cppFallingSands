#ifndef GAME_HPP
#define GAME_HPP

#include <vector>
#include <random>
#include <string>
#include <iostream>
#include <SDL2/SDL.h>
#include "../Elements/BaseElements/Pixel.hpp"
#include "../Elements/Sand.hpp"

class Game {
public:
    Game(int vecWidth, int vecHeight, int rendererScalex, int rendererScaley);
    ~Game();

    void init(const std::string* title);
    void handleEvents(const uint8_t &xScale, const uint8_t &yScale);
    void update();
    void render();
    void clean();
    void setRunning(bool running) { isRunning = running; }
    bool getRunning() const { return isRunning; }
    void swapElements(int x, int y, int x2, int y2);
    double randomnumber();
private:
    int vecWidth;
    int vecHeight;
    int rendererScalex;
    int rendererScaley;
    bool isRunning;
    SDL_Window* window;
    SDL_Renderer* renderer;
    std::vector<std::vector<Pixel *>> vec;
};

#endif // GAME_HPP
