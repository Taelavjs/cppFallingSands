#ifndef PLAYER_HPP
#define PLAYER_HPP
#include "../Textures/Sprite.hpp"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <vector>
#include <iostream>
#include <string>

class Player {
public:
    Player(Sprite* sprite);
    ~Player();

    Sprite* getSprite(){
        return playerSprite;
    }

    void playerInputHandler(SDL_Event& e);
    void playerReleaseHandler(SDL_Event& e);
    void renderPlayer(SDL_Renderer* renderer);
    void update();

private:
    Sprite* playerSprite;
    std::string directionMove{"None"};
    int16_t x{0}, y{0};
    int16_t speed{3};
    bool dLeft{false}, dRight{false}, dUp{false}, dDown{false};
    
};

#endif /* PLAYER_HPP */
