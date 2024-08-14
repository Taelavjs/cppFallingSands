#ifndef PLAYER_HPP
#define PLAYER_HPP
#include "../Textures/Sprite.hpp"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <vector>
#include <iostream>
#include <string>
#include <tuple>
#include "../Elements/BaseElements/Pixel.hpp"
#include <stack>

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
    void update(std::vector<std::vector<Pixel *>> vec, SDL_Renderer* renderer, int vecWidth);
    std::tuple <int, int> getCoordinates(){
        return std::make_tuple(x, y);
    }

    std::tuple <int, int> getDimensions(){
        return std::make_tuple(xScale, yScale);
    }

    void handleCollision(SDL_Rect* colliderRect);
    SDL_Rect getPlayerRect(){return playerAABB;}
    std::stack<SDL_Rect> getStackRender(){return stckToRender;};


private:
    SDL_Rect playerAABB;

    std::stack<SDL_Rect> stckToRender;
    Sprite* playerSprite;
    std::string directionMove{"None"};
    double xVel{0}, yVel{0};
    int x{0}, y{0};
    int playerXCenter{0}, playerYCenter{0};
    double maxVel{2.5f};
    double acceleration{0.75f};
    double deacceleration{1.85f};
    int xScale{8}, yScale{8};
    double speed{3};
    bool dLeft{false}, dRight{false}, dUp{false}, dDown{false};
    
};

#endif /* PLAYER_HPP */
