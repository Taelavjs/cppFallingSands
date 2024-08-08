#ifndef PIXEL_HPP
#define PIXEL_HPP

#include <unordered_map>
#include <string>
#include <random>
#include <vector>
#include <SDL2/SDL.h> // Include SDL2 header for SDL_Color
#include <iostream> // Include SDL2 header for SDL_Color

// Default Base Class Of Every Pixel
class Pixel {

    // GETTERS ELEMENTAL FEATURES
public:
    bool getIsMoveable(){return isMoveable;}
    bool getIsLiquid(){return isLiquid;}
    bool getIsSolid(){return isSolid;}
    bool getIsGas(){return isGas;}
    bool getIsFlammable(){return isFlammable;}

    // SETTERS ELEMENTAL FEATURES
    void setIsMoveable(bool val){isMoveable= val;}
    void setIsLiquid(bool val){isLiquid= val;}
    void setIsSolid(bool val){isSolid = val;}
    void setIsGas(bool val){isGas= val;}
    void setIsFlammable(bool val){isFlammable= val;}

    // ELEMENTAL VARIABLES
private:
    bool isMoveable{false};
    bool isFlammable{false};
    bool isGas{false};
    bool isLiquid{false};
    bool isSolid{false};

    // GETTERS FUNCTIONS
public:
    bool getProcessed() { return isProcessed; }
    int getMass(){return mass;}
    bool getOnFire();
    uint32_t getColour() { return color;}
    double getChanceToIgnite(){return chanceToIgnite;}
    int getHp(){return hp;}
    int getDensity(){return density;}
    int getTerminalX(){return terminalX;}
    int getTerminalY(){return terminalY;}
    // SETTERS FUNCTIONS
public:
    void setProcessed(bool val) { isProcessed = val;}
    void setFlammable(bool val){isFlammable = val;}
    void setOnFire();
    void setHp(int health){hp = health;}
    void setMass(int val){mass = val;}
    void setChanceToIgnite(double val){chanceToIgnite = val;}
    void setDensity(int val){density = val;}
    void setColor(uint32_t val){color = val;}
    
private:
    int hp{5};
    bool onFire{false};
    uint32_t color{SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32), 255, 255, 255, 255)};
    double chanceToIgnite{0.9f};
    bool isProcessed{false};
    int mass{1};
    int density{0};
    int terminalX{3};
    int terminalY{3};

public:
    Pixel();
    virtual ~Pixel();
    virtual Pixel *clone() const = 0;
    virtual void update(std::vector<std::vector<Pixel *>> &vec, int &row, int &col, int &vecWidth, int &vecHeight) {};
    void swapElements(std::vector<std::vector<Pixel*>>& vec, int x1, int y1,int x2, int y2);

    // FIRE FUNCTIONALITY
public:
    virtual void ignite();
    virtual bool hit();
    virtual bool fireTick(std::vector<std::vector<Pixel *>> &vec, int row, int col, int vecHeight, Pixel *elm);
};

#endif // PIXEL_HPP
