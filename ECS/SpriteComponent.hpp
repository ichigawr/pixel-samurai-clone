#pragma once


#include <SDL2/SDL.h>
#include <map>

#include "Components.hpp"
#include "../TextureManager.hpp"
#include "Animation.hpp"


class SpriteComponent : public Component {
    private:
        TransformComponent *transform;
        SDL_Texture *texture;
        SDL_Rect srcRect, destRect;

        bool animated = false;
        int frames = 0;
        int speed = 100;

    public:
        int animIndex = 0;

        std::map<const char*, Animation> animations;

        SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;
    
        SpriteComponent() = default;

        SpriteComponent(const char* filePath) {
            setTexture(filePath);
        }

        SpriteComponent(const char* filePath, bool isAnimated) {
            animated = isAnimated;

            Animation idle = Animation(0, 8, 100);
            Animation run = Animation(1, 8, 100);

            animations.emplace("Idle", idle);
            animations.emplace("Run", run);
            
            Play("Idle");

            setTexture(filePath);
        }

        ~SpriteComponent() {
            SDL_DestroyTexture(texture);
        }

        void setTexture(const char* filePath) {
            texture = TextureManager::LoadTexture(filePath);
        }

        void init() override {
            transform = &entity->getComponent<TransformComponent>();

            srcRect.x = srcRect.y = 0;
            srcRect.w = transform->width;
            srcRect.h = transform->height;
        }

        void update() override {
            if (animated) {
                srcRect.x = srcRect.w * static_cast<int>((SDL_GetTicks() / speed) % frames);
            }

            srcRect.y = animIndex * transform->height;
            
            destRect.x = static_cast<int>(transform->position.x);
            destRect.y = static_cast<int>(transform->position.y);
            destRect.w = transform->width * transform->scale;
            destRect.h = transform->height * transform->scale;
        }

        void draw() override {
            TextureManager::Draw(texture, srcRect, destRect, spriteFlip);
        }

        void Play(const char* animName) {
            frames = animations[animName].frames;
            animIndex = animations[animName].index;
            speed = animations[animName].speed;
        }
};
