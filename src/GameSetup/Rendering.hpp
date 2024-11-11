#ifndef RENDERING_HPP
#define RENDERING_HPP
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

#include <iostream>
#include <vector>
#include "../Elements/BaseElements/Pixel.hpp"
#include "../Playables/Player.hpp"
#include <stack>
#include "../Utility/Velocity.hpp"
class Player;
class GlobalVariables;
class Rendering {
public:

    static void renderGrid(Chunk &vec, Player* player, Vector2D globalCoords);
    static void castRay(uint32_t *pixels,SDL_Renderer* renderer, const std::vector<std::vector<Pixel*>>& vec, int startX, int startY, int endX, int endY) ;
    static void castRays(uint32_t *pixels,SDL_Renderer* renderer, const std::vector<std::vector<Pixel*>>& vec, int row, int col) ;
    static SDL_Renderer* getRenderer(){
        return renderer;
    };
    static void setValues();
    static void renderPlayer(Player* player);
    static void showRendering();

private:

    Rendering();
    ~Rendering();
    static SDL_Window* window;
    static SDL_Renderer* renderer;
    static int offsetX;
    static int offsetY;
};

#endif /* RENDERING_HPP */
