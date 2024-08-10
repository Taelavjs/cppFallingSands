#include "Player.hpp"

Player::Player(Sprite* sprite):playerSprite(sprite)
{

}

Player::~Player(){
    delete playerSprite;
}

void Player::playerReleaseHandler(SDL_Event& e){
        if(e.key.keysym.sym == SDLK_RIGHT){
            dRight = false;
        } else if(e.key.keysym.sym == SDLK_LEFT){
            dLeft = false;
        } else if(e.key.keysym.sym == SDLK_UP){
            dUp = false;
        } else if(e.key.keysym.sym == SDLK_DOWN){
            dDown = false;
        }
}


void Player::playerInputHandler(SDL_Event& e){
    if(e.key.keysym.sym == SDLK_RIGHT){
        dRight = true;
    } else if(e.key.keysym.sym == SDLK_LEFT){
        dLeft = true;
    } else if(e.key.keysym.sym == SDLK_UP){
        dUp = true;
    } else if(e.key.keysym.sym == SDLK_DOWN){
        dDown = true;
    }
}

void Player::update(){
    x += (dRight) ? speed : 0;
    x -= (dLeft) ? speed : 0;
    y += (dDown) ? speed : 0;
    y -= (dUp) ? speed : 0;
}

void Player::renderPlayer(SDL_Renderer* renderer){
    SDL_Texture* playerTexture = playerSprite->runCycle();
    SDL_Rect* dst = new SDL_Rect{x, y, 50, 50};
    SDL_RenderCopy(renderer, playerTexture, NULL, dst);
    delete dst;
}
