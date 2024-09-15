#ifndef PLAYER_HPP
#define PLAYER_HPP
#include "../Textures/Sprite.hpp"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <vector>
#include <iostream>
#include <map>
#include <string>
#include <tuple>
#include "../Elements/BaseElements/Pixel.hpp"
#include <stack>
#include "../Utility/Velocity.hpp"
#include "PlayerHandlers/PlayerStateManager.hpp"
#include "../Textures/SpriteAnimation.hpp"
#include "../Textures/Animation.hpp"
class Sprite;
class Player {
public:
    Player(Sprite* sprite);
    ~Player();

    Sprite* getSprite(){
        return playerSprite;
    }

    void playerInputHandler(SDL_Event& e);
    void playerReleaseHandler(SDL_Event& e);
    void renderPlayer(SDL_Renderer* renderer, int screenWidth);
    void update(std::vector<std::vector<Pixel *>> vec, SDL_Renderer* renderer, int vecWidth);
    std::tuple <int, int> getCoordinates(){
        return std::make_tuple(position.x, position.y);
    }

    std::tuple <int, int> getDimensions(){
        return std::make_tuple(playerScale.x, playerScale.y);
    }

    void handleCollision(SDL_Rect* colliderRect);
    SDL_Rect getPlayerRect(){return playerAABB;}
    std::stack<SDL_Rect> getStackRender(){return stckToRender;};
    void playerForcesInputs();
    void collisionHandler(int vecWidth, std::vector<std::vector<Pixel *>> vec);
    bool isFlipTexture();
    Velocity velocity;
    void resetPlayerColliders();

private:
    SDL_Rect playerAABB;
    SDL_Rect groundedRect;
    std::stack<SDL_Rect> stckToRender;
    Sprite* playerSprite;
    Vector2D position;
    Vector2D validPosition;
    Vector2D playerCenterPosition;
    Vector2D playerScale;
    bool dLeft{false}, dRight{false}, dUp{false}, dDown{false};
    bool isFlipped{false};
    std::map<playerStates, Animation> animations;
    bool isGrounded{false};
    PlayerStateManager stateManager;
    playerStates prev;
};

#endif /* PLAYER_HPP */
