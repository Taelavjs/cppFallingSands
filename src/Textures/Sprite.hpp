#ifndef SPRITE_HPP
#define SPRITE_HPP
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <vector>
#include <iostream>
#include "../GameSetup/Rendering.hpp"


class Sprite{
public:
    Sprite(char* srcPath);
    Sprite(char* srcPath, int& width, int& height, int& rows, int& cols);
    ~Sprite();

    // GETTERS/SETTERS
    SDL_Texture* getTexture();
    std::vector<std::vector<SDL_Rect>> getRects();

    // DEBUG
    void showSpriteSheet(int &rows, int &cols);
private:
    SDL_Texture* texture;
    std::vector<std::vector<SDL_Rect>> rectSheet;
    SDL_Surface* surface;

    // ???
    int rowOneCounter{0};
    int rowTwoCounter{1};

    int rowFourCounter{0};
};

#endif /* SPRITE_HPP */
