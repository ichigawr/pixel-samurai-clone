#pragma once

#include <SDL2/SDL.h>

#include "ECS.hpp"


class TileComponent : public Component {
    public:
        SDL_Texture *texture;
        SDL_Rect srcRect, destRect;

        TileComponent() = default;

        ~TileComponent() {
            SDL_DestroyTexture(texture);
        }

        TileComponent(int srcX, int srcY, int xpos, int ypos, const char* filePath) {
            texture = TextureManager::LoadTexture(filePath);

            srcRect.x = srcX;
            srcRect.y = srcY;
            srcRect.w = srcRect.h = 32;

            destRect.x = xpos;
            destRect.y = ypos;
            destRect.w = destRect.h = 64;
        }

        void draw() override {
            TextureManager::Draw(texture, srcRect, destRect, SDL_FLIP_NONE);
        }
};