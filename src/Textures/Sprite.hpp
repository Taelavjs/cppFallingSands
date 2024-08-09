#ifndef SPRITE_HPP
#define SPRITE_HPP
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <vector>
#include <iostream>

class Sprite {
public:
    Sprite(char* srcPath, SDL_Renderer* renderer);
    Sprite(char* srcPath, SDL_Renderer* renderer, int& width, int& height, int& rows, int& cols);
    ~Sprite();

    SDL_Texture* getTexture(){
        return texture;
    }

    SDL_Texture* blinkCycle();
    SDL_Texture* runCycle();

private:
    SDL_Texture* texture;
    std::vector<std::vector<SDL_Texture*>> textureSheet;
    SDL_Surface* surface;
    int rowOneCounter{0};
    int rowTwoCounter{1};

    int rowFourCounter{0};
};

#endif /* SPRITE_HPP */
