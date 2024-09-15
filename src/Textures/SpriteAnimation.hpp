#ifndef SPRITEANIMATION
#define SPRITEANIMATION
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "Animation.hpp"
#include "Sprite.hpp"
#include <string>
class SpriteAnimation{
public:
    SpriteAnimation() = delete;

    static Animation createAnimation(std::string title, std::vector<SDL_Rect> rects, SDL_Texture *texture, int noFrames, int fps, bool isLoop);
    
};

#endif /* SPRITEANIMATION */
