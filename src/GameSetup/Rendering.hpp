#ifndef RENDERING_HPP
#define RENDERING_HPP
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

#include <iostream>
#include <vector>
#include "../Elements/BaseElements/Pixel.hpp"
#include "../Playables/Player.hpp"
#include <stack>

class Rendering {
public:
    Rendering(int vecWidth, int vecHeight, const std::string* title, int scaleX, int scaleY) ;
    ~Rendering();
    void renderGrid(std::vector<std::vector<Pixel *>> &vec, Player* player);
    void castRay(uint32_t *pixels,SDL_Renderer* renderer, const std::vector<std::vector<Pixel*>>& vec, int screenWidth, int screenHeight, int startX, int startY, int endX, int endY) ;
    void castRays(uint32_t *pixels,SDL_Renderer* renderer, const std::vector<std::vector<Pixel*>>& vec, int screenWidth, int screenHeight, int row, int col) ;
    SDL_Renderer* getRenderer();
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    int rendererScalex;
    int rendererScaley;
    int screenHeight;
    int screenWidth;
};

#endif /* RENDERING_HPP */
