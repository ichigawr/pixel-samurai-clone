#pragma once

#include <SDL2/SDL.h>
#include <map>

#include "Components.hpp"
#include "../TextureManager.hpp"
#include "Animation.hpp"
#include "../AssetManager.hpp"


class SpriteComponent : public Component {
    private:
        TransformComponent *transform;
        SDL_Texture *texture;
        SDL_Rect srcRect, destRect;

        bool animated = false;
        int frames = 0;
        int speed = 100;

    public:
        int aniIndex = 0;
        const char* currentAni;
        Uint32 aniStartTime = 0;

        std::map<const char*, Animation> animations;

        SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;
    
        SpriteComponent() = default;

        SpriteComponent(std::string id) {
            animated = false;
            setTexture(id);
        }

        SpriteComponent(std::string id, bool isAnimated, std::map<const char*, Animation> objectAnimations) {
            animated = isAnimated;
            animations = objectAnimations;
            
            Play("Idle");

            setTexture(id);
        }

        ~SpriteComponent() {    }

        void setTexture(std::string id) {
            texture = Game::assets->GetTexture(id);
        }

        void init() override {
            transform = &entity->getComponent<TransformComponent>();

            srcRect.x = srcRect.y = 0;
            srcRect.w = transform->width;
            srcRect.h = transform->height;
        }

        void update() override {
            if (animated)
                srcRect.x = srcRect.w * static_cast<int>(((SDL_GetTicks() - aniStartTime) / speed) % frames);

            srcRect.y = aniIndex * transform->height;

            srcRect.w = animations[currentAni].frameWidth;
            srcRect.h = animations[currentAni].frameHeight;

            destRect.x = static_cast<int>(transform->position.x) - Game::camera.x;

            if (spriteFlip == SDL_FLIP_HORIZONTAL)
                destRect.x -= animations[currentAni].frameWidth - 64;

            destRect.y = static_cast<int>(transform->position.y) - Game::camera.y - animations[currentAni].frameHeight + 64;

            destRect.w = animations[currentAni].frameWidth * transform->scale;
            destRect.h = animations[currentAni].frameHeight * transform->scale;
        }

        void draw() override {
            TextureManager::Draw(texture, srcRect, destRect, spriteFlip);
        }

        void Play(const char* aniName) {
            currentAni = aniName;

            frames = animations[aniName].frames;
            aniIndex = animations[aniName].index;
            speed = animations[aniName].speed;

            if (aniIndex >= 2)
                aniStartTime = SDL_GetTicks();
        }
};
