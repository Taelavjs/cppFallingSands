#ifndef PIXEL_HPP
#define PIXEL_HPP

#include <unordered_map>
#include <string>
#include <SDL2/SDL.h> // Include SDL2 header for SDL_Color
#include <iostream> // Include SDL2 header for SDL_Color

// Default Base Class Of Every Pixel
class Pixel {
public:
    Pixel();
    virtual ~Pixel();
    virtual void update(std::vector<std::vector<Pixel *>> &vec, int &row, int &col, int &vecWidth, int &vecHeight) {};
    virtual void setProcessed(bool tf) { isProcessed = tf; }
    virtual bool getProcessed() { return isProcessed; }
    virtual Pixel *clone() const = 0;
    virtual uint32_t getColour() { return SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32), 255, 255, 255, 255); }
    virtual bool isMoveable(){return false;}
    
protected:
    bool isProcessed{false};
    int mass{1};
    int g{3};
    SDL_Color elmColor = {.r = 255, .g = 255, .b = 255};
    
    int density{0};

    // PixelType pixelType;
};

#endif // PIXEL_HPP
