#include "Player.hpp"

Player::Player(Sprite* sprite):playerSprite(sprite), velocity(), stateManager(), position(0 ,0), validPosition(0,0),playerScale(16 , 16), playerCenterPosition(0 ,0)
{
    SDL_Texture* texture = playerSprite -> getTexture();
    prev = playerStates::Falling;
    
    animations[playerStates::Idle] = SpriteAnimation::createAnimation("Idle", playerSprite -> getRects()[1], texture, 2, 2, true);
    animations[playerStates::Running] = SpriteAnimation::createAnimation("Running", playerSprite -> getRects()[3], texture, 4, 18, true);
    animations[playerStates::Jumping] = SpriteAnimation::createAnimation("Jumping", playerSprite -> getRects()[5], texture, 7, 14, false);
    animations[playerStates::Falling] = SpriteAnimation::createAnimation("Falling", playerSprite -> getRects()[7], texture, 7, 8, false);
}

Player::~Player(){
    delete playerSprite;
}

void Player::playerReleaseHandler(SDL_Scancode e){
        if(e == SDL_SCANCODE_RIGHT){
            dRight = false;
        }  
        if(e == SDL_SCANCODE_LEFT){
            dLeft = false;
        }  
        if(e == SDL_SCANCODE_UP){
            dUp = false;
        }  
        if(e == SDL_SCANCODE_DOWN){
            dDown = false;
        }
}


void Player::playerInputHandler(SDL_Scancode e){
    if(e == SDL_SCANCODE_RIGHT){
        dRight = true;
    }  
    if(e == SDL_SCANCODE_LEFT){
        dLeft = true;
    }  
    if(e == SDL_SCANCODE_UP){
        dUp = true;
    }  
    if(e == SDL_SCANCODE_DOWN){
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
            velocity.addForce(3, 90);
        }
    }


}

void Player::resetPlayerColliders(){
    // Player Collider
    playerCenterPosition.x = position.x + (playerScale.x/2) - 1;
    playerCenterPosition.y = position.y + (playerScale.y/2);
    playerAABB = {(int)position.x + 5, (int)position.y, (int)playerScale.x - 10, (int)playerScale.y}; // leniency

    // check for isGrounded
    groundedRect = {(int)playerCenterPosition.x - 1, (int)(playerCenterPosition.y + playerScale.y/2), 5, 5};
}

void Player::checkAreaCollision(bool &isBlockInPlayer, std::vector<SDL_Rect> &collisions, int vecWidth, Chunk& vec){
    SDL_Rect collisionResult;

    int jOffset = (vec.getGlobalCoords().y * -vecWidth);
    int iOffset = (vec.getGlobalCoords().x * -vecWidth);
    isBlockInPlayer = false;
    if(vec.size() == 0) return;

    collisions.clear();
    for(int l = 0; l < 8; l++){
        for(int i = position.x + iOffset + 3; i < position.x + iOffset + 3 + playerScale.x - 6; ++i){
            for(int j = position.y + jOffset; j < position.y + jOffset + playerScale.y; ++j){
                SDL_Rect cube = {i - iOffset, j - jOffset, 1, 1};
                if(!(i > 0 && j > 0 && j < vecWidth && i < vecWidth)) {
                    continue;
                }
                if(vec[j][i] != nullptr && vec[j][i]->getIsSolid() && SDL_IntersectRect(&cube, &playerAABB, &collisionResult)){
                    collisions.push_back(cube);
                    isBlockInPlayer = true;
                }
            }
        }
    }
}

void Player::collisionHandler(int vecWidth, Chunk& vec)
{
    resetPlayerColliders();
    validPosition = position;
    int jOffset = (vec.getGlobalCoords().y * -vecWidth);
    int iOffset = (vec.getGlobalCoords().x * -vecWidth);

    // flag to check for collisions with environment once
    bool wasGrounded{false};
    bool isBlockInPlayer{false};
    std::vector<SDL_Rect> collisions;
    checkAreaCollision(isBlockInPlayer, collisions, vecWidth, vec);

    
    // Calculate average displacement from players center


    if(!isBlockInPlayer){
        SDL_Rect col;
        for(int i = playerCenterPosition.x + iOffset - 4; i < playerCenterPosition.x + iOffset + 8; ++i){
            if(wasGrounded) break;
            for(int j = playerCenterPosition.y + jOffset + playerScale.y/2; j < playerCenterPosition.y + jOffset + 2 + playerScale.y/2; ++j){
                if(wasGrounded) break;

                if(!(i > 0 && j > 0 && j < vecWidth && i < vecWidth)) continue;
                SDL_Rect cube = {i - iOffset, j - jOffset, 1, 1};
                stckToRender.push(cube);
                if(vec.size() == 0) continue;

                if(vec[j][i] != nullptr && vec[j][i]->getIsSolid() && SDL_IntersectRect(&cube, &groundedRect, &col)){
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
        Vector2D playerVelocity = velocity.getVelocity();
        float yBlocksMove{-playerVelocity.y};
        float xBlocksMove{playerVelocity.x};
        position.y += yBlocksMove;
        resetPlayerColliders();
        checkAreaCollision(isBlockInPlayer, collisions, vecWidth, vec);
        if(isBlockInPlayer){
            position.y = validPosition.y;
            velocity.setVelocity(velocity.getVelocity().x, 0);
        }

        resetPlayerColliders();
        position.x += xBlocksMove;
        resetPlayerColliders();
        checkAreaCollision(isBlockInPlayer, collisions, vecWidth, vec);
        if(isBlockInPlayer){
            position.x = validPosition.x;
            velocity.setVelocity(0, velocity.getVelocity().y);
        }

        resetPlayerColliders();


    }

    // Move player horizontal/vertical if x/y is larger
    int tries = 0;
    if(isBlockInPlayer && tries < 10){
        double dispX = 0.0f;
        double dispY = 0.0f;
        for (auto i = collisions.begin(); i != collisions.end(); ++i)
        {  
            dispY += playerCenterPosition.y - i->y;
            dispX += playerCenterPosition.x - i->x;
        }
        dispY = (dispY * 1) / collisions.size();
        dispX = (dispX * 1) / collisions.size();
        std::cout << "Y DISP  : " << dispY << '\n';
        std::cout << "X DISP  : " << dispX << '\n';
        position.y -=  2.5;
        position.x += velocity.getVelocity().x;
        resetPlayerColliders();

        checkAreaCollision(isBlockInPlayer, collisions, vecWidth, vec);
        if(!isBlockInPlayer) return;
        
        resetPlayerColliders();
        checkAreaCollision(isBlockInPlayer, collisions, vecWidth, vec);
        if(isBlockInPlayer){
            position.y = validPosition.y;
            velocity.setVelocity(velocity.getVelocity().x, 0);
        }

        resetPlayerColliders();
        resetPlayerColliders();
        checkAreaCollision(isBlockInPlayer, collisions, vecWidth, vec);
        if(isBlockInPlayer){
            position.x = validPosition.x;
            velocity.setVelocity(0, velocity.getVelocity().y);
        }

        resetPlayerColliders();
    }

    if(tries >= 10){
        position = validPosition;
    }



}

void Player::update(Chunk& vec, SDL_Renderer* renderer, int vecWidth)
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

void Player::renderPlayer(SDL_Renderer* renderer, int screenWidth){
    if(prev != stateManager.getCurrentState()){
        animations[prev].resetCounter();
        prev = stateManager.getCurrentState();
    }
    SDL_Texture* texture = playerSprite -> getTexture();
    SDL_Rect* rect = animations[stateManager.getCurrentState()].play();

    SDL_Rect* dst = new SDL_Rect{0 + screenWidth/2, 0 + screenWidth/2, (int)playerScale.x, (int)playerScale.y};
    SDL_RendererFlip flip;
    if(isFlipTexture()){
        flip = SDL_FLIP_HORIZONTAL;
    } else {
        flip = SDL_FLIP_NONE;
    }

    SDL_RenderCopyEx(renderer, texture, rect, dst, NULL, NULL, flip);
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

