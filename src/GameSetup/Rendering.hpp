#ifndef RENDERING_HPP
#define RENDERING_HPP
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

#include <iostream>
#include <vector>
#include "../Elements/BaseElements/Pixel.hpp"
#include "../Playables/Player.hpp"
#include <stack>
class Player;
class Rendering {
public:

    static void renderGrid(std::vector<std::vector<Pixel *>> &vec, Player* player);
    static void castRay(uint32_t *pixels,SDL_Renderer* renderer, const std::vector<std::vector<Pixel*>>& vec, int screenWidth, int screenHeight, int startX, int startY, int endX, int endY) ;
    static void castRays(uint32_t *pixels,SDL_Renderer* renderer, const std::vector<std::vector<Pixel*>>& vec, int screenWidth, int screenHeight, int row, int col) ;
    static SDL_Renderer* getRenderer(){
        return renderer;
    };
    static void setValues(int vecWidth, int vecHeight, const std::string* title, int scaleX, int scaleY);

private:

    Rendering();
    ~Rendering();
    static SDL_Window* window;
    static SDL_Renderer* renderer;
    static int rendererScalex;
    static int rendererScaley;
    static int screenHeight;
    static int screenWidth;
    static const std::string* title;
};

#endif /* RENDERING_HPP */
