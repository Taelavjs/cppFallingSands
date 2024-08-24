#include "Sprite.hpp"

Sprite::Sprite(char* srcPath){
    SDL_Renderer* renderer = Rendering::getRenderer();
    surface = IMG_Load(srcPath); 
    if (!surface) {
        std::cerr << "Failed to load surface: " << IMG_GetError() << std::endl;
        return;
    }

    texture = SDL_CreateTextureFromSurface(renderer, surface); 
    SDL_FreeSurface(surface);  
    if (!texture) {
        std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
    }
}

Sprite::Sprite(char* srcPath, int& width, int& height, int& rows, int& cols)
{
    SDL_Renderer* renderer = Rendering::getRenderer();
    textureSheet.resize(rows);
    for (int i = 0; i < rows; ++i) {
        textureSheet[i].resize(cols, nullptr);
    }

    surface = IMG_Load(srcPath);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    SDL_SetRenderTarget(renderer, nullptr);
    SDL_RenderClear(renderer);  
    
    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {

            textureSheet[y][x] = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);

            SDL_SetTextureBlendMode(textureSheet[y][x], SDL_BLENDMODE_BLEND);
            SDL_Rect srcRect = {x * width, y * height, width, height}; 
            SDL_SetRenderTarget(renderer, textureSheet[y][x]);

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
            SDL_RenderCopy(renderer, texture, &srcRect, nullptr);
        }
    }

    SDL_SetRenderTarget(renderer, nullptr);  // Reset the rendering target after the loop
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    int tileSize = 32;
    int spacing = 0;
    for(int i = 0; i < rows; ++i)
    {
        for(int j = 0; j < cols; ++j)
        {
            SDL_Rect rect = {
                j * (tileSize + spacing), 
                i * (tileSize + spacing), 
                tileSize , 
                tileSize
            };
            SDL_RenderCopy(renderer, textureSheet[i][j], NULL, &rect);
        }
    }
    SDL_RenderPresent(renderer);
    SDL_PumpEvents();
    SDL_Delay(500);

}


Sprite::~Sprite(){
    SDL_DestroyTexture(texture);

    for (size_t i = 0; i < textureSheet.size(); ++i) {
        for (size_t j = 0; j < textureSheet[i].size(); ++j) {
            SDL_DestroyTexture(textureSheet[i][j]);  
        }
        textureSheet[i].clear();  
    }

    textureSheet.clear(); 
}
SDL_Texture* Sprite::blinkCycle(){
    if(rowTwoCounter >= 2) {
        rowTwoCounter = 0;
    }

    rowTwoCounter++;
    return textureSheet[1][rowTwoCounter-1];

}

SDL_Texture* Sprite::runCycle(){
    if(rowFourCounter >=5) {
        rowFourCounter = 0;
    }
    return textureSheet[3][rowFourCounter++];

}
