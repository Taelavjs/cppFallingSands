#ifndef PIXEL_HPP
#define PIXEL_HPP

#include <unordered_map>
#include <string>

enum class PixelType {
    Sand,
    Water,
    Air
};

std::string pixelTypeToString(PixelType type);

class Pixel {
public:
    Pixel(PixelType type);
    ~Pixel();

    void updateVelocity();
    void resetVelocity();
    int getBlocksToFall();
    void reset();
    

private:
    static std::unordered_map<PixelType, int> createPixelTypeMap();
    static const std::unordered_map<PixelType, int> pixelTypes;
    int mass;
    int velocity{0};
    int g;
    PixelType pixelType;
};

#endif // PIXEL_HPP
