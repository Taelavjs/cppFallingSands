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
        }  
        if(e.key.keysym.sym == SDLK_LEFT){
            dLeft = false;
        }  
        if(e.key.keysym.sym == SDLK_UP){
            dUp = false;
        }  
        if(e.key.keysym.sym == SDLK_DOWN){
            dDown = false;
        }
}


void Player::playerInputHandler(SDL_Event& e){
    if(e.key.keysym.sym == SDLK_RIGHT){
        dRight = true;
    }  
    if(e.key.keysym.sym == SDLK_LEFT){
        dLeft = true;
    }  
    if(e.key.keysym.sym == SDLK_UP){
        dUp = true;
    }  
    if(e.key.keysym.sym == SDLK_DOWN){
        dDown = true;
    }
}

void Player::update(){
    xVel += (dRight) ? (xVel < -acceleration ? deacceleration + acceleration : acceleration) : 0;
    xVel -= (dLeft) ? (xVel > acceleration ? deacceleration + acceleration : acceleration) : 0;
    yVel += (dDown) ? (yVel < -acceleration ? deacceleration + acceleration : acceleration) : 0;
    yVel -= (dUp) ? (yVel > acceleration ? deacceleration + acceleration : acceleration) : 0;

    x += (std::abs(xVel) > maxVel) ? (xVel > 0 ? maxVel : -maxVel) : xVel;
    y += (std::abs(yVel) > maxVel) ? (yVel > 0 ? maxVel : -maxVel) : yVel;
}

void Player::renderPlayer(SDL_Renderer* renderer){
    SDL_Texture* playerTexture = playerSprite->runCycle();
    SDL_Rect* dst = new SDL_Rect{x, y, xScale, yScale};
    SDL_RenderCopy(renderer, playerTexture, NULL, dst);
    delete dst;
    if (!(dRight || dLeft)) {
        if (std::abs(xVel) <= acceleration) {
            xVel = 0;
        } else {
            xVel -= (xVel > 0 ? deacceleration : -deacceleration);
        }
    }
    if (!(dUp || dDown)) {
        if (std::abs(yVel) <= acceleration) {
            yVel = 0;
        } else {
            yVel -= (yVel > 0 ? deacceleration : -deacceleration);
        }
    }
}
