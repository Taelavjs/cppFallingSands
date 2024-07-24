#ifndef RENDERING_HPP
#define RENDERING_HPP
#include "SDL2/SDL.h"
#include <iostream>
#include <vector>
#include "../Elements/BaseElements/Pixel.hpp"

class Rendering {
public:
    Rendering(int vecWidth, int vecHeight, const std::string* title, int scaleX, int scaleY) ;
    ~Rendering();
    void renderGrid(std::vector<std::vector<Pixel *>> &vec);
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    int rendererScalex;
    int rendererScaley;
    int screenHeight;
    int screenWidth;
};

#endif /* RENDERING_HPP */
