#include "Player.hpp"

Player::Player(Sprite* sprite):playerSprite(sprite), velocity(), stateManager()
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

void Player::playerForcesInputs(){
    if(dLeft){
        velocity.addForce(3, 180);
    }
    if(dRight){
        velocity.addForce(3, 0);
    }
    if(dUp){
        playerStates currentState = stateManager.getCurrentState();
        if(currentState == playerStates::Idle || currentState == playerStates::Running){
            velocity.addForce(10, 90);
        }
    }


}

void Player::resetPlayerColliders(){
    // Player Collider
    playerXCenter = x + (xScale/2) - 1;
    playerYCenter = y + (yScale/2);
    playerAABB = {x + 5, y, xScale - 10, yScale}; // leniency

    // check for isGrounded
    groundedRect = {playerXCenter - 2, playerYCenter + yScale/2, 4, 2};
}

void Player::collisionHandler(int vecWidth, std::vector<std::vector<Pixel *>> vec)
{
    resetPlayerColliders();
    SDL_Rect collisionResult;

    // Slopes Collider
    SDL_Rect slopeLeft = {validX - 1, validY + yScale, 1, 1};
    SDL_Rect slopeRight = {validX + xScale + 1, validY + yScale, 1, 1};

    stckToRender.push(slopeLeft);
    stckToRender.push(slopeRight);

    // flag to check for collisions with environment once
    bool wasGrounded{false};
    bool isBlockInPlayer{false};
    for(int l = 0; l < 8; l++){
        for(int i = x + 3; i < x + 3 + xScale - 6; ++i){
            if(isBlockInPlayer) break;
            for(int j = y; j < y + yScale; ++j){
                if(isBlockInPlayer) break;

                if(!(i > 0 && j > 0 && j < vecWidth && i < vecWidth)) continue;

                SDL_Rect cube = {i, j, 1, 1};

                if(vec[j][i] != nullptr && vec[j][i]->getIsSolid() && SDL_IntersectRect(&cube, &playerAABB, &collisionResult)){
                    
                    if(j < playerYCenter + (0.25f * yScale)){
                        resetPlayerColliders();
                        isBlockInPlayer = true;
                    } else {
                        y--;
                        resetPlayerColliders();
                    }

                }
            }
        }
    }




    for(int i = playerXCenter - 4; i < playerXCenter + 8; ++i){
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

    if(!isGrounded && stateManager.getCurrentState() == playerStates::Running)
    {
        velocity.addForce(7, 270);
    }

        // Calculate blocks to move
    if(isBlockInPlayer){
        x = validX;
        y = validY;
        velocity.resetVelocity();
    } else {
        validX = x;
        validY = y;
        Vector2D playerVelocity = velocity.getVelocity();
        float yBlocksMove{-playerVelocity.y};
        float xBlocksMove{playerVelocity.x};
        y += yBlocksMove;
        x += xBlocksMove;
    }
}

void Player::update(std::vector<std::vector<Pixel *>> vec, SDL_Renderer* renderer, int vecWidth)
{
    float prevX = x;
    float prevY = y;
    velocity.velocityTick();
    //Debugging rendering
    while(!stckToRender.empty()){
        stckToRender.pop();
    }
    playerForcesInputs();
    collisionHandler(vecWidth, vec);
                
    resetPlayerColliders();
    stckToRender.push(groundedRect);
    stckToRender.push(playerAABB);
    stateManager.updatePlayerState(velocity.getVelocity(), isGrounded);
}

void Player::renderPlayer(SDL_Renderer* renderer){
    SDL_Texture* playerTexture = playerSprite->runCycle();
    SDL_Rect* dst = new SDL_Rect{validX, validY, xScale, yScale};
    SDL_RendererFlip flip;
    if(isFlipTexture()){
        flip = SDL_FLIP_HORIZONTAL;
    } else {
        flip = SDL_FLIP_NONE;
    }

    SDL_RenderCopyEx(renderer, playerTexture, NULL, dst, NULL, NULL, flip);
    delete dst;
}

void Player::handleCollision(SDL_Rect* colliderRect){
}

bool Player::isFlipTexture(){
    Vector2D playerVelocity = velocity.getVelocity();
    if(playerVelocity.x == 0) return isFlipped;
    if(playerVelocity.x < 0){
        isFlipped = true;
    } else {
        isFlipped = false;
    }
    return isFlipped;
}

