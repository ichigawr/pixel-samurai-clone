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
        std::string currentAni;
        Uint32 aniStartTime = 0;

        std::map<std::string, Animation> animations;
        std::vector<int> srcY;

        SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;
    
        SpriteComponent() = default;

        SpriteComponent(std::string id) {
            animated = false;
            setTexture(id);
        }

        SpriteComponent(std::string id, bool isAnimated, std::map<std::string, Animation> objectAnimations) {
            animated = isAnimated;
            animations = objectAnimations;
            
            srcY.resize(animations.size());
            srcY[0] = 0;

            int i = 1;
            for (auto& ani : animations) {
                ani.second.index = i - 1;
                srcY[i] = srcY[i - 1] + ani.second.frameHeight;
                i++;
            }
            
            Play("Idle");

            setTexture(id);
        }

        ~SpriteComponent() {    }

        SDL_Rect getDestRect() {
            return destRect;
        }

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

            srcRect.y = srcY[aniIndex];

            srcRect.w = animations[currentAni].frameWidth;
            srcRect.h = animations[currentAni].frameHeight;

            destRect.x = static_cast<int>(transform->position.x) - Game::camera.x;

            if (spriteFlip == SDL_FLIP_HORIZONTAL) {
                destRect.x -= animations[currentAni].frameWidth - 64 + animations[currentAni].marginLeft;
            } else destRect.x += animations[currentAni].marginLeft;

            destRect.y = static_cast<int>(transform->position.y) - Game::camera.y - animations[currentAni].frameHeight + 48 - animations[currentAni].marginBottom;

            destRect.w = animations[currentAni].frameWidth * transform->scale;
            destRect.h = animations[currentAni].frameHeight * transform->scale;
        }

        void draw() override {
            TextureManager::Draw(texture, srcRect, destRect, spriteFlip);
        }

        void Play(std::string aniName) {
            currentAni = aniName;

            frames = animations[aniName].frames;
            aniIndex = animations[aniName].index;
            speed = animations[aniName].speed;

            if (aniName != "Run")
                aniStartTime = SDL_GetTicks();
        }
};
