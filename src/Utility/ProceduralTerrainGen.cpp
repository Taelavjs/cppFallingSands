#include "ProceduralTerrainGen.hpp"


std::vector<float> ProceduralTerrainGen::createNoise(int w, int h){
    FastNoiseLite noise;
    noise.SetNoiseType(FastNoiseLite::NoiseType_Cellular);
    noise.SetFrequency(0.1);
    noise.SetFractalType(FastNoiseLite::FractalType_None);
    noise.SetCellularDistanceFunction(FastNoiseLite::CellularDistanceFunction_Hybrid);
    noise.SetCellularReturnType(FastNoiseLite::CellularReturnType_CellValue);
    noise.SetCellularJitter(1.9);
    noise.SetDomainWarpType(FastNoiseLite::DomainWarpType_OpenSimplex2);
    noise.SetDomainWarpAmp(5.0f);
    noise.SetFractalType(FastNoiseLite::FractalType_DomainWarpIndependent);
    noise.SetFractalLacunarity(1.920);
    noise.SetFractalOctaves(3);
    noise.SetFractalGain(2.180f);

    // Gather noise data
    std::vector<float> noiseData(w * h);
    int index = 0;

    for (int y = 0; y < w; y++)
    {
        for (int x = 0; x < h; x++)
        {
            noiseData[index++] = noise.GetNoise((float)x, (float)y);
        }
    }

    return noiseData;
}




// void ProceduralTerrainGen::iterThroughPixels(void (*func)(int &, int &, uint32_t *, uint32_t *), uint32_t *pixels, uint32_t *newPixels){
//     for (int row = 0; row < vecWidth; ++row)
//     {
//         for (int col = 0; col < vecWidth; ++col)
//         {
//             func(row, col, pixels, newPixels);
//         }
//     }
// }
// void waterLevelLimit(int &row, int &col, uint32_t *pixels, uint32_t *newPixels){
//     uint32_t color{};
//     int yellowNeighborCount{0};
//     int greenNeighborCount{0};
//     int BlueNeighborCount{0};
//     for (int i = -1; i <= 1; ++i)
//     {
//         for (int j = -1; j <= 1; ++j)
//         {
//             if (i == 0 && j == 0)
//                 continue;
//             int neighborRow = row + i;
//             int neighborCol = col + j;

//             if (neighborRow >= 0 && neighborRow < vecHeight && neighborCol >= 0 && neighborCol < vecWidth)
//             {
//                 if (pixels[neighborRow * vecWidth + neighborCol] == greenColor)
//                 {
//                     greenNeighborCount++;
//                 } else if(pixels[neighborRow * vecWidth + neighborCol] == yellowColor){
//                     yellowNeighborCount++;

//                 } else if(pixels[neighborRow * vecWidth + neighborCol] == blueColor){
//                     BlueNeighborCount++;
//                 }
//             }
//         }
//     }

//     if (greenNeighborCount < 3)
//     {
//         color = blueColor;
//     }
//     else
//     {
//         color = greenNeighborCount  >= 4 ? greenColor : vertColor;
//         if(BlueNeighborCount > 2 && BlueNeighborCount <= 5){
//             color = yellowColor;
//         } else if(BlueNeighborCount > 2){
//             color = blueColor;
//         }
//     }
//     newPixels[row * vecWidth + col] = color;
// }
// void colorCells(int &row, int &col, uint32_t *pixels, uint32_t *newPixels){
//     uint32_t color{};

//     int neighborCount{0};
//     for (int i = -1; i <= 1; ++i)
//     {
//         for (int j = -1; j <= 1; ++j)
//         {
//             if (i == 0 && j == 0)
//                 continue;
//             int neighborRow = row + i;
//             int neighborCol = col + j;

//             if (neighborRow >= 0 && neighborRow < vecHeight && neighborCol >= 0 && neighborCol < vecWidth)
//             {
//                 if (pixels[neighborRow * vecWidth + neighborCol] == whiteColor || 
//                     pixels[neighborRow * vecWidth + neighborCol] == greenColor)
//                 {
//                     neighborCount++;
//                 }
//             }
//         }
//     }
//     if (neighborCount < liveNeightborFreshold)
//     {
//         color = blueColor;
//     }
//     else
//     {
//         color = neighborCount == 8 ? greenColor : yellowColor;
//     }

//     newPixels[row * vecWidth + col] = color;
// }
// void placeLiveDeadCells(int &row, int &col, uint32_t *pixels, uint32_t *newPixels){
//     uint32_t color{};

//     int neighborCount{0};
//     for (int i = -1; i <= 1; ++i)
//     {
//         for (int j = -1; j <= 1; ++j)
//         {
//             if (i == 0 && j == 0)
//                 continue;
//             int neighborRow = row + i;
//             int neighborCol = col + j;

//             if (neighborRow >= 0 && neighborRow < vecHeight && neighborCol >= 0 && neighborCol < vecWidth)
//             {
//                 if (pixels[neighborRow * vecWidth + neighborCol] == whiteColor)
//                 {
//                     neighborCount++;
//                 }
//             }
//         }
//     }
//     if (neighborCount < liveNeightborFreshold)
//     {
//         color = blackColor;
//     }
//     else
//     {
//         color = whiteColor;
//     }

//     newPixels[row * vecWidth + col] = color;
// };

// void ProceduralGeneratedMap(uint32_t *pixels)
// {
//     SDL_Window *window = SDL_CreateWindow("title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, vecWidth * rendererScalex, vecHeight * rendererScaley, SDL_WINDOW_SHOWN);
//     SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
//     SDL_RenderSetScale(renderer, rendererScalex, rendererScaley);
//     SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
//     SDL_RenderPresent(renderer);
//     SDL_PumpEvents();

//     SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, vecWidth, vecHeight);
//     std::random_device rd;
//     std::mt19937 gen(rd());
//     std::uniform_real_distribution<> dis(0, 1); 

//     for (int row = 0; row < vecWidth; ++row)
//     {
//         for (int col = 0; col < vecWidth; ++col)
//         {
//             uint32_t color{};
//             if (dis(gen) > liveCellChance)
//             {
//                 color = blackColor;
//             }
//             else
//             {
//                 color = whiteColor;
//             }

//             pixels[row * vecWidth + col] = color;
//         }
//     }

//     displayPixels(texture, pixels, renderer);

//     uint32_t *newPixels = new uint32_t[vecWidth * vecHeight];

//     for (int i = 0; i < 9; ++i)
//     {
//         iterThroughPixels(placeLiveDeadCells, pixels, newPixels);
//         std::swap(pixels, newPixels);
//         displayPixels(texture, pixels, renderer);
//         SDL_Delay(1000);
//     }

//     iterThroughPixels(colorCells, pixels, newPixels);
//     std::swap(pixels, newPixels);
//     displayPixels(texture, pixels, renderer);

//     // for(int p = 0; p < 2; ++p){
//     //     iterThroughPixels(waterLevelLimit, pixels, newPixels);
//     //     std::swap(pixels, newPixels);
//     //     displayPixels(texture, pixels, renderer);
//     // }

//     delete[] newPixels;
//     delete[] pixels;
//     SDL_DestroyTexture(texture);
//     SDL_Delay(0);
//     SDL_DestroyWindow(window);
// };
