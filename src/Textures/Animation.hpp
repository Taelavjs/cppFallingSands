#ifndef ANIMATION
#define ANIMATION
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <string>
#include <vector>

class Animation{
public:
    Animation(std::string iTitle, std::vector<SDL_Rect> iFrameReel, SDL_Texture* iTexture, int fps);
    Animation();
    ~Animation();
    SDL_Rect* play();

private:
    void updateFrame();
    std::string title{""};
    int animationRate{0};
    int currentFrame{0};
    int totalFrames{0};
    double deltaTime{0.0f};
    double prevTime{0.0f};
    SDL_Texture *texture;
    std::vector<SDL_Rect> frameRects;
};

#endif /* ANIMATION */
