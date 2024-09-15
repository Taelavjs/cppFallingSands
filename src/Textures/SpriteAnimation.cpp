#include "SpriteAnimation.hpp"

Animation SpriteAnimation::createAnimation(std::string title, std::vector<SDL_Rect> rects, SDL_Texture *texture, int noFrames, int fps){
    std::vector<SDL_Rect> temp(rects.begin(), rects.begin() + noFrames);
    Animation newAnimation(title, temp, texture, fps);
    return newAnimation;
}
