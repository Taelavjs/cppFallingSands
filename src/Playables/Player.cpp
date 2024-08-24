#include "Player.hpp"

Player::Player(Sprite* sprite):playerSprite(sprite), velocity()
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

void Player::update(std::vector<std::vector<Pixel *>> vec, SDL_Renderer* renderer, int vecWidth)
{
    float prevX = x;
    float prevY = y;
    velocity.velocityTick();

    if(dLeft){
        velocity.addForce(3, 180);
    }
    if(dRight){
        velocity.addForce(3, 0);
    }
    if(dDown){
        velocity.addForce(3, 270);
    }
    if(dUp){
        velocity.addForce(3, 90);
    }
    //Debugging rendering
    while(!stckToRender.empty()){
        stckToRender.pop();
    }
    // Player Collider

    playerXCenter = x + (xScale/2) - 1;
    playerYCenter = y + (yScale/2);


    playerAABB = {x + 3, y, xScale - 6, yScale}; // leniency

    // check for isGrounded
    groundedRect = {playerXCenter, playerYCenter + yScale/2, 2, 2};
    SDL_Rect collisionResult;

    // flag to check for collisions with environment once
    bool wasGrounded{false};
    bool isBlockInPlayer{false};

    for(int i = x + 3; i < x + 3 + xScale - 6; ++i){
        if(isBlockInPlayer) break;
        for(int j = y; j < y + yScale; ++j){
            if(isBlockInPlayer) break;

            if(!(i > 0 && j > 0 && j < vecWidth && i < vecWidth)) continue;

            SDL_Rect cube = {i, j, 1, 1};

            if(vec[j][i] != nullptr && vec[j][i]->getIsSolid() && SDL_IntersectRect(&cube, &playerAABB, &collisionResult)){
                

                // Reset player center coords
                playerXCenter = x + (xScale/2) - 1;
                playerYCenter = y + (yScale/2);

                // Reset players center coords
                playerAABB = {x + 3, y, xScale - 6, yScale}; // leniency

                // Reset players grounded rect
                groundedRect = {playerXCenter, playerYCenter + yScale/2, 2, 2};

                isBlockInPlayer = true;
                break;
            }
        }
    }

    for(int i = playerXCenter; i < playerXCenter + 2; ++i){
        if(wasGrounded) break;
        for(int j = playerYCenter + yScale/2; j < playerYCenter + 2 + yScale/2; ++j){
            if(wasGrounded) break;

            if(!(i > 0 && j > 0 && j < vecWidth && i < vecWidth)) continue;
            SDL_Rect cube = {i, j, 1, 1};
            stckToRender.push(cube);

            if(vec[j][i] != nullptr && vec[j][i]->getIsSolid() && SDL_IntersectRect(&cube, &groundedRect, &collisionResult)){
                wasGrounded = true;
                break;
            }
        }
    }

    if(wasGrounded){
        isGrounded = true;
    } else {
        isGrounded = false;
    }
    velocity.setIsGrounded(isGrounded);


    // ********************




    // ********************

    // Calculate blocks to move
    if(isBlockInPlayer){
        x = validX;
        y = validY;
    } else {
        validX = x;
        validY = y;
        Vector2D playerVelocity = velocity.getVelocity();
        float yBlocksMove{-playerVelocity.y};
        float xBlocksMove{playerVelocity.x};
        y += yBlocksMove;
        x += xBlocksMove;
        velocity.resetVelocity();
    }
                
    playerAABB = {validX + 3, validY, xScale - 6, yScale}; // leniency
    groundedRect = {playerXCenter, playerYCenter + yScale/2, 2, 2};
    playerXCenter = validX + (xScale/2) - 1;
    playerYCenter = validY + (yScale/2);

    stckToRender.push(groundedRect);
    stckToRender.push(playerAABB);

}

void Player::renderPlayer(SDL_Renderer* renderer){
    SDL_Texture* playerTexture = playerSprite->runCycle();
    SDL_Rect* dst = new SDL_Rect{validX, validY, xScale, yScale};
    SDL_RenderCopy(renderer, playerTexture, NULL, dst);
    delete dst;
}

void Player::handleCollision(SDL_Rect* colliderRect){
}
