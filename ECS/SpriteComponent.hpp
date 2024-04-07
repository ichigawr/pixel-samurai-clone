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
        int animIndex = 0;

        std::map<const char*, Animation> animations;

        SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;
    
        SpriteComponent() = default;

        SpriteComponent(std::string id) {
            setTexture(id);
        }

        SpriteComponent(std::string id, bool isAnimated) {
            animated = isAnimated;

            Animation idle = Animation(0, 8, 100, 64, 64);
            Animation run = Animation(1, 8, 100, 64, 64);
            Animation attack = Animation(2, 4, 50, 150, 86);

            animations.emplace("Idle", idle);
            animations.emplace("Run", run);
            animations.emplace("Attack", attack);
            
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
            if (animated) {
                srcRect.x = srcRect.w * static_cast<int>((SDL_GetTicks() / speed) % frames);
            }

            srcRect.y = animIndex * transform->height;

            destRect.x = static_cast<int>(transform->position.x) - Game::camera.x;
            destRect.y = static_cast<int>(transform->position.y) - 86 + 64 - Game::camera.y;
            
            // destRect.x = static_cast<int>(transform->position.x) - Game::camera.x;
            // destRect.y = static_cast<int>(transform->position.y) - Game::camera.y;
            // destRect.w = transform->width * transform->scale;
            // destRect.h = transform->height * transform->scale;
        }

        void draw() override {
            TextureManager::Draw(texture, srcRect, destRect, spriteFlip);
        }

        void Play(const char* animName) {
            frames = animations[animName].frames;
            animIndex = animations[animName].index;
            speed = animations[animName].speed;
            srcRect.w = animations[animName].frameWidth;
            srcRect.h = animations[animName].frameHeight;
            // destRect.x = static_cast<int>(transform->position.x) - 155 + 64 - Game::camera.x;
            // destRect.y = static_cast<int>(transform->position.y) - 86 + 64 - Game::camera.y;
            destRect.w = animations[animName].frameWidth;
            destRect.h = animations[animName].frameHeight;
        }
};
