#include "Player.hpp"

Player::Player(Sprite* sprite):playerSprite(sprite), velocity(), stateManager(), position(0 ,0), validPosition(0,0),playerScale(16 , 16), playerCenterPosition(0 ,0)
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
    playerCenterPosition.x = position.x + (playerScale.x/2) - 1;
    playerCenterPosition.y = position.y + (playerScale.y/2);
    playerAABB = {(int)position.x + 5, (int)position.y, (int)playerScale.x - 10, (int)playerScale.y}; // leniency

    // check for isGrounded
    groundedRect = {(int)playerCenterPosition.x - 1, (int)(playerCenterPosition.y + playerScale.y/2), 2, 5};
}

void Player::collisionHandler(int vecWidth, std::vector<std::vector<Pixel *>> vec)
{
    resetPlayerColliders();
    SDL_Rect collisionResult;

    // flag to check for collisions with environment once
    bool wasGrounded{false};
    bool isBlockInPlayer{false};
    for(int l = 0; l < 8; l++){
        for(int i = position.x + 3; i < position.x + 3 + playerScale.x - 6; ++i){
            if(isBlockInPlayer) break;
            for(int j = position.y; j < position.y + playerScale.y; ++j){
                if(isBlockInPlayer) break;

                if(!(i > 0 && j > 0 && j < vecWidth && i < vecWidth)) continue;

                SDL_Rect cube = {i, j, 1, 1};

                if(vec[j][i] != nullptr && vec[j][i]->getIsSolid() && SDL_IntersectRect(&cube, &playerAABB, &collisionResult)){
                    
                    if(j < playerCenterPosition.y + (0.25f * playerScale.y)){ // If collision is not at players feet
                        resetPlayerColliders();
                        isBlockInPlayer = true;
                    } else {
                        position.y--;
                        resetPlayerColliders();
                    }

                }
            }
        }
    }




    for(int i = playerCenterPosition.x - 4; i < playerCenterPosition.x + 8; ++i){
        if(wasGrounded) break;
        for(int j = playerCenterPosition.y + playerScale.y/2; j < playerCenterPosition.y + 2 + playerScale.y/2; ++j){
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
        position.x = validPosition.x;

        if(!wasGrounded){
            Vector2D playerVelocity = velocity.getVelocity();
            if(playerVelocity.y > 0){
                position.y = validPosition.y;
            } else {
                validPosition.y = position.y;
                float yBlocksMove{-playerVelocity.y};
                position.y += yBlocksMove;
                velocity.setVelocity(0, playerVelocity.y);
                resetPlayerColliders();
            }
        } else {
            position.y = validPosition.y;
            velocity.setVelocity(0, 0);
        }
    } else {
        validPosition.x = position.x;
        validPosition.y = position.y;
        Vector2D playerVelocity = velocity.getVelocity();
        float yBlocksMove{-playerVelocity.y};
        float xBlocksMove{playerVelocity.x};
        position.y += yBlocksMove;
        position.x += xBlocksMove;
    }
}

void Player::update(std::vector<std::vector<Pixel *>> vec, SDL_Renderer* renderer, int vecWidth)
{
    velocity.velocityTick();
    //Debugging rendering
    while(!stckToRender.empty()){
        stckToRender.pop();
    }
    playerForcesInputs();
    collisionHandler(vecWidth, vec);
                
    resetPlayerColliders();
    stateManager.updatePlayerState(velocity.getVelocity(), isGrounded);
}

void Player::renderPlayer(SDL_Renderer* renderer){
    SDL_Texture* playerTexture = playerSprite->runCycle();
    SDL_Rect* dst = new SDL_Rect{(int)validPosition.x, (int)validPosition.y, (int)playerScale.x, (int)playerScale.y};
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

