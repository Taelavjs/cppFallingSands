#ifndef PIXEL_HPP
#define PIXEL_HPP

#include <unordered_map>
#include <string>
#include <vector>
#include <SDL2/SDL.h> // Include SDL2 header for SDL_Color
#include <iostream> // Include SDL2 header for SDL_Color

// Default Base Class Of Every Pixel
class Pixel {
public:
    Pixel();
    virtual ~Pixel();
    virtual Pixel *clone() const = 0;
    virtual void update(std::vector<std::vector<Pixel *>> &vec, int &row, int &col, int &vecWidth, int &vecHeight) {};
public:
    virtual void setProcessed(bool tf) { isProcessed = tf; }
    virtual bool getProcessed() { return isProcessed; }
    virtual uint32_t getColour() { return SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32), 255, 255, 255, 255); }
    virtual bool isMoveable();
    virtual bool isLiquid();
    virtual bool isSolid(){return false;};
    virtual bool isGas(){return false;};
    virtual bool isFlammable(){return flammable;};
    bool setFlammable(bool val){flammable = val;};
    void swapElements(std::vector<std::vector<Pixel*>>& vec, int x1, int y1,int x2, int y2);
    virtual int getDensity(){return 0;};
    virtual int getMass(){return mass;};

protected:
    bool isProcessed{false};
    int mass{1};
    int g{3};
    SDL_Color elmColor = {.r = 255, .g = 255, .b = 255};
    int density{0};
    int terminalX{3};
    int terminalY{3};
    bool moveable{false};
    bool flammable{false};

};

#endif // PIXEL_HPP
