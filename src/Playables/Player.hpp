#ifndef PLAYER_HPP
#define PLAYER_HPP
#include "../Textures/Sprite.hpp"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <vector>
#include <iostream>
#include <string>
#include <tuple>

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
    std::tuple <int, int> getCoordinates(){
        return std::make_tuple(x, y);
    }

    std::tuple <int, int> getDimensions(){
        return std::make_tuple(xScale, yScale);
    }

private:

    Sprite* playerSprite;
    std::string directionMove{"None"};
    double xVel{0}, yVel{0};
    int x{0}, y{0};
    double maxVel{2.5f};
    double acceleration{0.75f};
    double deacceleration{1.85f};
    int xScale{8}, yScale{8};
    double speed{3};
    bool dLeft{false}, dRight{false}, dUp{false}, dDown{false};
    
};

#endif /* PLAYER_HPP */