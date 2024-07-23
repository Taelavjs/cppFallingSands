#ifndef PIXEL_HPP
#define PIXEL_HPP

#include <unordered_map>
#include <string>
#include <SDL2/SDL.h> // Include SDL2 header for SDL_Color

// Default Base Class Of Every Pixel
class Pixel {
public:
    Pixel();
    virtual ~Pixel();
    virtual SDL_Color getColor(){ return color;};
    virtual void update(std::vector<std::vector<Pixel *>> &vec, int &row, int &col, int &vecWidth, int &vecHeight) {};
    virtual void setProcessed(bool tf) { isProcessed = tf; }
    virtual bool getProcessed() { return isProcessed; }
    // void updateVelocity();
    // void resetVelocity();
    // int getBlocksToFall();
protected:
    bool isProcessed{false};

//     static std::unordered_map<PixelType, int> createPixelTypeMap();
//     static const std::unordered_map<PixelType, int> pixelTypes;
// int velocity{0};
    int mass{1};
    int g{3};
    SDL_Color color = {.r = 255, .g = 255, .b = 255};
    // PixelType pixelType;
};

#endif // PIXEL_HPP
