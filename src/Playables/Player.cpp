#include "Player.hpp"

Player::Player(Sprite* sprite):playerSprite(sprite)
{

}

Player::~Player(){
    delete playerSprite;
}

