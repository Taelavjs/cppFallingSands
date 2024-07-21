#include "Pixel.hpp"
#include <algorithm>
#include <iostream>

std::string pixelTypeToString(PixelType type) {
    switch (type) {
        case PixelType::Sand:
            return "Sand";
        case PixelType::Water:
            return "Water";
        case PixelType::Air:
            return "Air";
        default:
            return "Unknown";
    }
}

std::unordered_map<PixelType, int> Pixel::createPixelTypeMap() {
    std::unordered_map<PixelType, int> umap;
    umap[PixelType::Sand] = 8;
    umap[PixelType::Water] = 3;
    umap[PixelType::Air] = 1;
    return umap;
}

const std::unordered_map<PixelType, int> Pixel::pixelTypes = Pixel::createPixelTypeMap();

Pixel::Pixel(PixelType type)
    : mass(pixelTypes.at(type)), g(8), pixelType(type) {}

Pixel::~Pixel() {}

void Pixel::updateVelocity() {
    velocity += g/3;
}

void Pixel::resetVelocity() {
    velocity = 0;
}

int Pixel::getBlocksToFall() {
    int acceleration = g / mass;
    velocity += acceleration;
    int blocksToFall = std::max(1, std::min(14, velocity / 4)); // Divide by a higher value to reduce the falling speed
    return blocksToFall;
}

void Pixel::reset() {
    pixelType = PixelType::Air;
    velocity = 0;
    mass = 1;
}
