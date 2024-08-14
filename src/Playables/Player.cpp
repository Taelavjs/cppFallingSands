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

void Player::update(std::vector<std::vector<Pixel *>> vec, SDL_Renderer* renderer, int vecWidth){
    
    int pX = x;
    int pY = y;
    xVel += (dRight) ? (xVel < -acceleration ? deacceleration + acceleration : acceleration) : 0;
    xVel -= (dLeft) ? (xVel > acceleration ? deacceleration + acceleration : acceleration) : 0;
    yVel += (dDown) ? (yVel < -acceleration ? deacceleration + acceleration : acceleration) : 0;
    yVel -= (dUp) ? (yVel > acceleration ? deacceleration + acceleration : acceleration) : 0;

    x += (std::abs(xVel) > maxVel) ? (xVel > 0 ? maxVel : -maxVel) : xVel;
    y += (std::abs(yVel) > maxVel) ? (yVel > 0 ? maxVel : -maxVel) : yVel;
    playerXCenter = x + (xScale/2);
    playerYCenter = y + (yScale/2);


    playerAABB = {x+2, y+2, xScale -4 , yScale - 4}; // leniency

    while(!stckToRender.empty()){
        stckToRender.pop();
    }
    for(int i = playerXCenter + xScale/2 - 2; i > playerXCenter - xScale/2 + 2; --i){
        for(int j = playerYCenter + yScale/2 - 2; j > playerYCenter - yScale/2 + 2; --j){
            if(!(i > 0 && j > 0)) continue;
            SDL_Rect cube = {i, j, 1, 1};
            //stckToRender.push(cube);
            SDL_Rect res;

            // if(vec[j][i] != nullptr && vec[j][i]->getIsSolid() && SDL_IntersectRect(&cube, &playerAABB, &res)){
            //     if(j > playerYCenter + yScale/2){
            //         y += j - (playerYCenter + yScale/2);
            //         playerAABB = {x+2, y+2, xScale -4 , yScale - 4}; // leniency
            //         continue;
            //     }
            //     x = pX;
            //     y = pY;
            //     yVel = 0;
            //     xVel = 0;
            // }
        }
    }

    int distLeftFoot{};
    int distRightFoot{};

    int leftFootXPos = (playerXCenter + 1);
    int leftFootYPos = (playerYCenter + 2);

    int rightFootXPos = (playerXCenter - 2);
    int rightFootYPos = (playerYCenter + 2);

    int topLeftXPos = (playerXCenter + 1);
    int topLeftYPost = (playerYCenter - 3);

    int topRightXPos = (playerXCenter - 2);
    int topRightYPost = (playerYCenter- 3);

    int leftXPos = (playerXCenter + 2);
    int leftYPos = (playerYCenter);

    int rightXPos = (playerXCenter - 3);
    int rightYPos = (playerYCenter);

    if(leftXPos >= 0 && leftXPos < vecWidth - 1 && leftYPos >= 0 && leftYPos < vecWidth - 1){
        if(leftYPos < vecWidth && (vec[leftYPos][leftXPos] == nullptr || !vec[leftYPos][leftXPos]->getIsSolid()) ){

        } else {
            y = pY;
            x = pX - 2;
            xVel = 0;
            yVel = 0;
        }

    } else {
        y = pY;
        x = pX ;
        xVel = 0;
        yVel = 0;
    }


    if(rightXPos >= 0 && rightXPos < vecWidth - 1 && rightYPos >= 0 && rightYPos < vecWidth - 1){
        if(rightYPos < vecWidth && (vec[rightYPos][rightXPos] == nullptr || !vec[rightYPos][rightXPos]->getIsSolid()) ){

        } else {
            y = pY;
            x = pX + 2;
            xVel = 0;
            yVel = 0;
        }
    } else {
        y = pY;
        x = pX ;
        xVel = 0;
        yVel = 0;
    }

    if(topLeftXPos >= 0 && topLeftXPos < vecWidth - 1 && topLeftYPost >= 0 && topLeftYPost < vecWidth - 1){
        if(topLeftYPost < vecWidth && (vec[topLeftYPost][topLeftXPos] == nullptr || !vec[topLeftYPost][topLeftXPos]->getIsSolid()) ){

        } else {
            y = pY;
            x = pX - 1;
            xVel = 0;
            yVel = 0;
        }

    } else {
        y = pY;
        x = pX ;
        xVel = 0;
        yVel = 0;
    }

    if(topRightXPos >= 0 && topRightXPos < vecWidth - 1 && topRightYPost >= 0 && topRightYPost < vecWidth - 1){
        if(topRightYPost < vecWidth && (vec[topRightYPost][topRightXPos] == nullptr || !vec[topRightYPost][topRightXPos]->getIsSolid()) ){

        } else {
            y = pY;
            x = pX + 1;
            xVel = 0;
            yVel = 0;
        }
    } else {
        y = pY;
        x = pX ;
        xVel = 0;
        yVel = 0;
    }

    while(!stckToRender.empty()){
        stckToRender.pop();
    }

    SDL_Rect foot1 = {rightFootXPos, rightFootYPos, 1, 1};
    SDL_Rect foot2 = {leftFootXPos, leftFootYPos, 1, 1};
    SDL_Rect foot3 = {topLeftXPos, topLeftYPost, 1, 1};
    SDL_Rect foot4 = {topRightXPos, topRightYPost, 1, 1};
    stckToRender.push(foot1);
    stckToRender.push(foot2);
    stckToRender.push(foot3);
    stckToRender.push(foot4);

    int xP{leftFootXPos};
    int yP{leftFootYPos};
    if(xP >= 0 && xP < vecWidth - 1 && yP >= 0 && yP < vecWidth - 1){
        while(yP < vecWidth && (vec[yP][xP] == nullptr || !vec[yP][xP]->getIsSolid()) && std::abs(yP - leftFootYPos) < 4){
            yP++;
        }

        if(vec[yP][xP] != nullptr){
            distRightFoot = std::abs(yP - leftFootYPos);
        } else {
            distRightFoot = 4;
        }
    }

    xP= rightFootXPos;
    yP = rightFootYPos;
    if(xP >= 0 && xP < vecWidth - 1 && yP >= 0 && yP < vecWidth - 1){
    while(yP < vecWidth && (vec[yP][xP] == nullptr || !vec[yP][xP]->getIsSolid()) && std::abs(yP - rightFootYPos) < 4){
        yP++;
    }

    if(vec[yP][xP] != nullptr){
        distLeftFoot = std::abs(yP - rightFootYPos);
    } else {
        distLeftFoot = 4;
    }
    }
    if(distLeftFoot > 2 && distRightFoot > 2){
        //player should be falling
    } else if(std::abs(distLeftFoot - distRightFoot) <= 2){
        if(std::abs(distLeftFoot - distRightFoot) == 0){
            y = pY;
            x = pX;
            xVel = 0;
            yVel = 0;
            return;
        }
        if(yVel < 0){
            y -= std::abs(distLeftFoot - distRightFoot);

        } else {
            y += std::abs(distLeftFoot - distRightFoot);

        }
    }
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

void Player::handleCollision(SDL_Rect* colliderRect){
    x += (x - colliderRect->x > 0) ? colliderRect->w : -colliderRect->w;
    y += (y - colliderRect->y > 0) ? colliderRect->h : -colliderRect->h;
}
