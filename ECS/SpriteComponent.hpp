#pragma once

#include "Components.hpp"
#include "SDL2/SDL.h"


class SpriteComponent : public Component {
    private:
        PositionComponent *position;
        SDL_Texture *texture;
        SDL_Rect srcRect, destRect;

    public:
        SpriteComponent() = default;

        SpriteComponent(const char* filePath) {
            setTexture(filePath);
        }

        ~SpriteComponent() {
            SDL_DestroyTexture(texture);
        }

        void setTexture(const char* filePath) {
            texture = TextureManager::LoadTexture(filePath);
        }

        void init() override {
            position = &entity->getComponent<PositionComponent>();

            srcRect.x = srcRect.y = 0;
            srcRect.w = srcRect.h = 32;
            destRect.w = destRect.h = 64;
        }

        void update() override {
            destRect.x = position->x();
            destRect.y = position->y();
        }

        void draw() override {
            TextureManager::Draw(texture, srcRect, destRect);
        }
};